// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUpMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "black_money/black_moneyGameMode.h"
#include "GameFramework/PlayerController.h"


void UStartUpMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UButton* StartButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Start"))))
	{
		FScriptDelegate StartDelegate;
		StartDelegate.BindUFunction(this, "StartButtonClicked");
		StartButton->OnClicked.Add(StartDelegate);
	}

	// 新增：绑定Quit按钮
	if (UButton* QuitButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Quit"))))
	{
		FScriptDelegate QuitDelegate;
		QuitDelegate.BindUFunction(this, "QuitButtonClicked");
		QuitButton->OnClicked.Add(QuitDelegate);
	}

	// 新增：绑定设置按钮
	if (UButton* SetButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Set"))))
	{
		FScriptDelegate SetDelegate;
		SetDelegate.BindUFunction(this, "SettingButtonClicked");
		SetButton->OnClicked.Add(SetDelegate);
	}
}

void UStartUpMenu::StartButtonClicked()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Clicked"));
	// 恢复世界运行
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGamePaused(World, false);

		// 恢复输入模式为游戏+UI
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeGameAndUI());
			PC->bShowMouseCursor = false; // 隐藏鼠标光标（根据需求调整）
		}

		// 步骤1：关闭启动菜单（从视口移除）
		if (this->IsInViewport())
		{
			this->RemoveFromViewport();  // 启动菜单不再显示
		}

		// 步骤2：获取GameMode并触发状态栏显示 CharacterMenu
		if (Ablack_moneyGameMode* GameMode = Cast<Ablack_moneyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode->ShowStatusBar();  // 启动菜单关闭后，才显示状态栏
		}
	}
}

// 新增：Quit按钮逻辑（实现暂停）
void UStartUpMenu::QuitButtonClicked()
{
	// 暂停世界
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGamePaused(World, true);

		// 设置输入模式为仅UI（确保能操作UI）
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Game Paused"));
			// 退出游戏（编辑器中会停止运行，打包后会关闭程序）
			UKismetSystemLibrary::QuitGame(
				GetWorld(),
				PC,
				EQuitPreference::Quit,  // 强制退出
				false  // 不显示确认对话框
			);
		}
	}
}

// 新增：设置按钮逻辑（弹出设置界面）
void UStartUpMenu::SettingButtonClicked()
{
	// 加载设置界面蓝图（请替换为你的设置界面蓝图路径）
	if (UClass* SettingWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_SettingMenu.BP_SettingMenu_C'")))
	{
		// 获取玩家控制器
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			// 创建设置界面实例并添加到视口
			UUserWidget* SettingMenuIns = CreateWidget(PC, SettingWidgetClass);
			if (SettingMenuIns)
			{
				SettingMenuIns->AddToViewport();
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("未找到设置界面蓝图，请检查路径"));
	}
}