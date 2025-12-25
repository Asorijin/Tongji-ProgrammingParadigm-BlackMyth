// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUpMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "black_money/black_moneyGameMode.h"
#include "GameFramework/PlayerController.h"
#include "FUIPanelManager.h"

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

	// 获取或创建
	//UUIBasePanel* Panel = FUIPanelManager::GetOrCreatePanel(GetWorld(), UStartUpMenu::StaticClass());

	// 或直接显示
	//FUIPanelManager::ShowPanel(GetWorld(), UStartUpMenu::StaticClass());
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
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false; // 隐藏鼠标光标（根据需求调整）
		}

		// 关闭启动菜单（从视口移除）
		if (this->IsInViewport())
		{
			this->RemoveFromViewport();  // 启动菜单不再显示
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
	UClass* SettingWidgetClass = LoadClass<UUIBasePanel>(
		nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_SettingMenu.BP_SettingMenu_C'")
	);

	// 2. 检查蓝图类是否加载成功
	if (!SettingWidgetClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("设置面板资源加载失败！请检查：1.路径是否正确 2.BP_SettingMenu是否继承UUIBasePanel"));
		return;
	}
	// 3. 获取世界上下文（空指针检查）
	UWorld* World = GetWorld();
	if (!World)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("获取World失败，无法创建设置面板"));
		return;
	}
	// 4. 核心修正：先通过PanelManager创建/获取面板实例，再显示（原函数缺少这步）
	UUIBasePanel* SettingPanelIns = FUIPanelManager::GetOrCreatePanel(World, SettingWidgetClass,FString("SettingWidget"));
	if (SettingPanelIns)
	{
		// 5. 显示面板（确保实例存在后调用ShowPanel）
		FUIPanelManager::ShowPanel(World, SettingWidgetClass, FString("SettingWidget"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("success"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("error"));
	}
}