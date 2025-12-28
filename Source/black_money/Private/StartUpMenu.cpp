// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUpMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "black_money/black_moneyGameMode.h"
#include "black_moneyGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "FUIPanelManager.h"

void UStartUpMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UButton* StartButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Start"))))
	{
		StartButton->OnClicked.RemoveAll(this);

		FScriptDelegate StartDelegate;
		StartDelegate.BindUFunction(this, "StartButtonClicked");
		StartButton->OnClicked.Add(StartDelegate);
	}

	// 新增：绑定Quit按钮
	if (UButton* QuitButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Quit"))))
	{
		QuitButton->OnClicked.RemoveAll(this);

		FScriptDelegate QuitDelegate;
		QuitDelegate.BindUFunction(this, "QuitButtonClicked");
		QuitButton->OnClicked.Add(QuitDelegate);
	}

	// 新增：绑定设置按钮
	if (UButton* SetButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_Set"))))
	{
		SetButton->OnClicked.RemoveAll(this);

		FScriptDelegate SetDelegate;
		SetDelegate.BindUFunction(this, "SettingButtonClicked");
		SetButton->OnClicked.Add(SetDelegate);
	}
}

void UStartUpMenu::StartButtonClicked()
{
	// 恢复世界运行
	if (UWorld* World = GetWorld())
	{

		UGameplayStatics::SetGamePaused(World, false);

		// 设置输入模式为游戏模式
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}

		// 通过面板管理器隐藏启动菜单
		FUIPanelManager::HidePanel(FString("StartUpMenu"));

		Cast<Ublack_moneyGameInstance>(GetGameInstance())->GetEventCenter()->ReadLastState();
		Cast<Ublack_moneyGameInstance>(GetGameInstance())->GetEventCenter()->SwitchToLevel();
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
	UClass* SettingWidgetClass = LoadClass<UUIBasePanel>(
		nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_SettingMenu.BP_SettingMenu_C'")
	);

	// 检查蓝图类是否加载成功
	if (!SettingWidgetClass)
	{
		return;
	}

	// 获取世界上下文（空指针检查）
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 核心修正：先通过PanelManager创建/获取面板实例，再显示
	UUIBasePanel* SettingPanelIns = FUIPanelManager::GetOrCreatePanel(World, SettingWidgetClass,FString("SettingWidget"));
	if (SettingPanelIns)
	{
		// 显示面板（确保实例存在后调用ShowPanel）
		FUIPanelManager::ShowPanel(World, SettingWidgetClass, FString("SettingWidget"));
	}
}
