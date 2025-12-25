// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "StartUpMenu.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // 绑定继续按钮
    if (UButton* ContinueButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Continue"))))
    {
        FScriptDelegate ContinueDelegate;
        ContinueDelegate.BindUFunction(this, "ContinueButtonClicked");
        ContinueButton->OnClicked.Add(ContinueDelegate);
    }

    // 绑定退出按钮（返回开始界面）
    if (UButton* QuitButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_PQuit"))))
    {
        FScriptDelegate QuitDelegate;
        QuitDelegate.BindUFunction(this, "QuitButtonClicked");
        QuitButton->OnClicked.Add(QuitDelegate);
    }

    // 绑定设置按钮
    if (UButton* SetButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_PSetting"))))
    {
        FScriptDelegate SetDelegate;
        SetDelegate.BindUFunction(this, "SettingButtonClicked");
        SetButton->OnClicked.Add(SetDelegate);
    }
}

void UPauseMenu::ContinueButtonClicked()
{
    if (UWorld* World = GetWorld())
    {
        // 恢复游戏运行
        UGameplayStatics::SetGamePaused(World, false);

        // 切换输入模式为游戏模式
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false; // 隐藏鼠标
        }

        // 关闭暂停菜单
        if (IsInViewport())
        {
            RemoveFromViewport();
        }
    }
}

void UPauseMenu::SettingButtonClicked()
{
    // 完全仿照StartUpMenu的设置按钮逻辑
    if (UClass* SettingWidgetClass = LoadClass<UUserWidget>(
        nullptr,
        TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_SettingMenu.BP_SettingMenu_C'")))
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            UUserWidget* SettingMenuIns = CreateWidget(PC, SettingWidgetClass);
            if (SettingMenuIns)
            {
                SettingMenuIns->AddToViewport();
            }
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("未找到设置菜单资源，请检查路径"));
    }
}

void UPauseMenu::QuitButtonClicked()
{
    if (UWorld* World = GetWorld())
    {
        // 关闭当前暂停菜单
        if (IsInViewport())
        {
            RemoveFromViewport();
        }

        // 显示开始菜单
        if (UClass* StartUpMenuClass = LoadClass<UStartUpMenu>(
            nullptr,
            TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_StartUpMenu.BP_StartUpMenu_C'"))) // 请替换为你的StartUpMenu蓝图路径
        {
            if (APlayerController* PC = World->GetFirstPlayerController())
            {
                // 切换输入模式为UI模式
                PC->SetInputMode(FInputModeUIOnly());
                PC->bShowMouseCursor = true;
                // 创建并显示开始菜单
                UStartUpMenu* StartUpMenuIns = CreateWidget<UStartUpMenu>(PC, StartUpMenuClass);
                if (StartUpMenuIns)
                {
                    StartUpMenuIns->AddToViewport();
                }
                // 确保游戏处于暂停状态（开始菜单通常需要暂停游戏）
                UGameplayStatics::SetGamePaused(World, true);
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("未找到开始菜单资源，请检查路径"));
        }
    }
}