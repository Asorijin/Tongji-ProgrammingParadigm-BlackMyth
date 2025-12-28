// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"
#include "FUIPanelManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "StartUpMenu.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // 绑定继续按钮
    if (UButton* ContinueButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Continue"))))
    {
        // 先清除当前对象在该按钮上的所有现有绑定，避免重复绑定
        ContinueButton->OnClicked.RemoveAll(this);

        FScriptDelegate ContinueDelegate;
        ContinueDelegate.BindUFunction(this, "ContinueButtonClicked");
        ContinueButton->OnClicked.Add(ContinueDelegate);
    }

    // 绑定退出按钮（返回开始界面）
    if (UButton* QuitButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_PQuit"))))
    {
        // 先清除当前对象在该按钮上的所有现有绑定
        QuitButton->OnClicked.RemoveAll(this);

        FScriptDelegate QuitDelegate;
        QuitDelegate.BindUFunction(this, "QuitButtonClicked");
        QuitButton->OnClicked.Add(QuitDelegate);
    }

    // 绑定设置按钮
    if (UButton* SetButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_PSetting"))))
    {
        // 先清除当前对象在该按钮上的所有现有绑定
        SetButton->OnClicked.RemoveAll(this);

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

        // 通过面板管理器隐藏暂停菜单
        FUIPanelManager::HidePanel(FString("PauseMenu"));
    }
}

void UPauseMenu::SettingButtonClicked()
{
    // 通过面板管理器加载并显示设置菜单
    UClass* SettingWidgetClass = LoadClass<UUIBasePanel>(
        nullptr,
        TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_SettingMenu.BP_SettingMenu_C'")
    );

    if (!SettingWidgetClass)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // 获取或创建设置面板实例并显示
    UUIBasePanel* PSettingPanelIns = FUIPanelManager::GetOrCreatePanel(World, SettingWidgetClass, FString("PSettingWidget"));
    if (PSettingPanelIns)
    {
        FUIPanelManager::ShowPanel(World, SettingWidgetClass, FString("PSettingWidget"));
    }
}

void UPauseMenu::QuitButtonClicked()
{
    if (UWorld* World = GetWorld())
    {
        // 通过面板管理器隐藏暂停菜单
        FUIPanelManager::HidePanel(FString("PauseMenu"));

        UClass* StartWidgetClass = LoadClass<UUIBasePanel>(
            nullptr,
            TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_StartUpMenu.BP_StartUpMenu_C'")
        );

        // 检查蓝图类是否加载成功
        if (!StartWidgetClass)
        {
            return;
        }

        if (!World)
        {
            return;
        }

        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            // 核心修正：先通过PanelManager创建/获取面板实例，再显示
            UUIBasePanel* StartPanelIns = FUIPanelManager::GetOrCreatePanel(World, StartWidgetClass, FString("StartUpMenu"));
            if (StartPanelIns)
            {
                // 显示面板（确保实例存在后调用ShowPanel）
                FUIPanelManager::ShowPanel(World, StartWidgetClass, FString("StartUpMenu"));

                // 暂停世界
                UGameplayStatics::SetGamePaused(GetWorld(), true);
                PC->SetInputMode(FInputModeGameOnly());
                PC->bShowMouseCursor = true;
            }
        }
    }
}