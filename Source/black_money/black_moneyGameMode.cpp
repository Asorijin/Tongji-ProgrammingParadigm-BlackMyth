// Copyright Epic Games, Inc. All Rights Reserved.

#include "black_moneyGameMode.h"
#include "black_moneyCharacter.h"
#include "black_moneyGameInstance.h"
#include "EventCenter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

Ablack_moneyGameMode::Ablack_moneyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	StartUpMenuIns = nullptr;
	StatusBarIns = nullptr;
	PauseMenuIns = nullptr;
	bIsPaused = false;
}

void Ablack_moneyGameMode::BeginPlay()
{
	Super::BeginPlay();
	// 展示界面，等待输入
	Ublack_moneyGameInstance* MyGI = Cast<Ublack_moneyGameInstance>(GetGameInstance());
	if (MyGI && MyGI->bIsFirstLaunch)
	{
		if (UClass* CustomWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_StartUpMenu.BP_StartUpMenu_C'")))
		{
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				StartUpMenuIns = CreateWidget(PC, CustomWidgetClass);
				if (StartUpMenuIns)
				{
					StartUpMenuIns->AddToViewport();
					// 暂停世界
					UGameplayStatics::SetGamePaused(GetWorld(), true);
					// 设置输入模式为仅UI（确保能点击按钮）
					PC->SetInputMode(FInputModeUIOnly());
					PC->bShowMouseCursor = true; // 显示鼠标光标
					MyGI->bIsFirstLaunch = false;
				}
			}
		}

		//// 加载开始菜单面板类（需确保BP_StartUpMenu继承自UUIBasePanel）
		//StartUpMenuClass = LoadClass<UUIBasePanel>(
		//	nullptr,
		//	TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_StartUpMenu.BP_StartUpMenu_C'")
		//);
		//if (StartUpMenuClass)
		//{
		//	// 通过面板管理器获取/创建实例并显示
		//	StartUpMenuIns = FUIPanelManager::GetOrCreatePanel(GetWorld(), StartUpMenuClass);
		//	FUIPanelManager::ShowPanel(GetWorld(), StartUpMenuClass);
		//	// 暂停游戏
		//	UGameplayStatics::SetGamePaused(GetWorld(), true);
		//	// 设置输入模式为仅UI，确保能点击按钮
		//	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		//	{
		//		PC->SetInputMode(FInputModeUIOnly());
		//		PC->bShowMouseCursor = true; // 显示鼠标光标
		//	}
		//	// 标记为非首次启动，后续关卡切换不再显示
		//	MyGI->bIsFirstLaunch = false;
		//}
	}

	if (DefaultMusic && GetWorld())
	{
		if (CurrentMusicComponent && CurrentMusicComponent->IsPlaying())
		{
			CurrentMusicComponent->Stop();
		}

		// 播放默认音乐（2D，全局背景音）
		CurrentMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), DefaultMusic);
		if (CurrentMusicComponent)
		{
			CurrentMusicComponent->bAllowSpatialization = false; // 确保是 2D 背景音乐
			CurrentMusicComponent->bAutoDestroy = true;          // 播放完自动销毁
			CurrentMusicComponent->bOverrideAttenuation = true;
			CurrentMusicComponent->AttenuationSettings = nullptr; // 无衰减
		}
	}
}

// 新增：实现显示状态栏的函数
void Ablack_moneyGameMode::ShowStatusBar()
{
	// 加载并显示状态栏（请替换为你的状态栏蓝图路径）
	if (UClass* StatusBarClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_CharacterMenu.BP_CharacterMenu_C'")))
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			StatusBarIns = CreateWidget(PC, StatusBarClass);
			if (StatusBarIns)
			{
				StatusBarIns->AddToViewport();  // 此时才显示状态栏
			}
		}
	}
}

void Ablack_moneyGameMode::ShowPauseMenu(bool bShow)
{
	if (bShow)
	{
		// 加载暂停UI（替换为你的暂停UI蓝图路径）
		if (UClass* PauseMenuClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_PauseMenu.BP_PauseMenu_C'")))
		{
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				PauseMenuIns = CreateWidget(PC, PauseMenuClass);
				if (PauseMenuIns)
				{
					PauseMenuIns->AddToViewport();
					// 暂停世界
					UGameplayStatics::SetGamePaused(GetWorld(), true);
					// 设置输入模式为仅UI（确保能点击按钮）
					PC->SetInputMode(FInputModeUIOnly());
					PC->bShowMouseCursor = true; // 显示鼠标光标
				}
			}
		}
	}
	else
	{
		// 隐藏暂停UI
		if (PauseMenuIns)
		{
			PauseMenuIns->RemoveFromViewport();
			PauseMenuIns = nullptr;
		}
		// 恢复输入模式为游戏+UI（允许角色控制）
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetInputMode(FInputModeGameAndUI());
			PC->bShowMouseCursor = false;
		}
	}
}

void Ablack_moneyGameMode::TogglePause()
{
	bIsPaused = !bIsPaused;
	UWorld* World = GetWorld();
	if (World)
	{
		// 切换游戏暂停状态
		UGameplayStatics::SetGamePaused(World, bIsPaused);
		// 显示/隐藏暂停UI
		ShowPauseMenu(bIsPaused);
	}

}

void Ablack_moneyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);
	if (Ublack_moneyGameInstance* GI = Cast<Ublack_moneyGameInstance>(GetGameInstance())) {
		UEventCenter* eventCenter = GI->GetEventCenter();
		eventCenter->GenerateActors();
	}
}

