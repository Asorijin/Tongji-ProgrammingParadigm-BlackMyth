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
		UClass* StartWidgetClass = LoadClass<UUIBasePanel>(
			nullptr,
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_StartUpMenu.BP_StartUpMenu_C'")
		);

		// 检查蓝图类是否加载成功
		if (!StartWidgetClass)
		{
			return;
		}

		// 获取世界上下文（空指针检查）
		UWorld* World = GetWorld();
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
				MyGI->bIsFirstLaunch = false;
			}
		}
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
	// 暂时使用系统函数加载并显示状态栏
	if (UClass* StatusBarClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_CharacterMenu.BP_CharacterMenu_C'")))
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			StatusBarIns = CreateWidget(PC, StatusBarClass);
			if (StatusBarIns)
			{
				StatusBarIns->AddToViewport(-1);  // 此时才显示状态栏
			}
		}
	}

	//// 别删！
	//// 以下改用FUIPanelManager的方式实现，需要在之后替换
	//// 使用PanelManager加载并显示状态条
	//UClass* StatusBarClass = LoadClass<UUIBasePanel>(
	//	nullptr,
	//	TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_CharacterMenu.BP_CharacterMenu_C'")
	//);

	//if (!StatusBarClass)
	//{
	//	return;
	//}

	//UWorld* World = GetWorld();
	//if (!World)
	//{
	//	return;
	//}

	//if (APlayerController* PC = World->GetFirstPlayerController())
	//{
	//	// 获取/创建面板实例
	//	StatusBarIns = FUIPanelManager::GetOrCreatePanel(World, StatusBarClass, FString("StatusBar"));
	//	if (StatusBarIns)
	//	{
	//		// 显示面板，设置层级为-1
	//		FUIPanelManager::ShowPanel(World, StatusBarClass, FString("StatusBar"));
	//	}
	//}
}

void Ablack_moneyGameMode::ShowPauseMenu(bool bShow)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ShowPauseMenu: World is null!"));
		return;
	}

	if (bShow)
	{
		// 初始化PauseMenuClass（只加载一次，避免重复加载）
		if (!PauseMenuClass)
		{
			// 修复LoadClass参数：第一个参数用GetOuter()而非nullptr，路径简化（UE推荐写法）
			static const FString PauseMenuPath = TEXT("/Game/UI/BP_PauseMenu.BP_PauseMenu_C");
			PauseMenuClass = LoadClass<UUIBasePanel>(GetOuter(), *PauseMenuPath);

			if (!PauseMenuClass)
			{
				UE_LOG(LogTemp, Error, TEXT("ShowPauseMenu: 加载PauseMenu失败，路径：%s"), *PauseMenuPath);
				return;
			}
		}

		APlayerController* PC = World->GetFirstPlayerController();
		if (!PC)
		{
			UE_LOG(LogTemp, Error, TEXT("ShowPauseMenu: 找不到PlayerController!"));
			return;
		}

		// 获取/创建面板实例（检查返回值）
		PauseMenuIns = FUIPanelManager::GetOrCreatePanel(World, PauseMenuClass, FString("PauseMenu"));
		if (PauseMenuIns)
		{
			FUIPanelManager::ShowPanel(World, PauseMenuClass, FString("PauseMenu"));

			// 暂停游戏（核心修复：明确暂停）
			UGameplayStatics::SetGamePaused(World, true);

			// 完善输入模式配置（避免UI交互崩溃）
			FInputModeUIOnly InputModeUI;
			InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways); // 锁定鼠标到视口
			InputModeUI.SetWidgetToFocus(PauseMenuIns->TakeWidget()); // 聚焦到暂停菜单Widget
			PC->SetInputMode(InputModeUI);

			// 启用鼠标交互
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ShowPauseMenu: 创建PauseMenu实例失败!"));
		}
	}
	else
	{
		// 隐藏暂停菜单（增加World检查）
		if (PauseMenuIns && World)
		{
			FUIPanelManager::HidePanel(FString("PauseMenu"));
			//PauseMenuIns->RemoveFromParent();
			//PauseMenuIns = nullptr;
		}

		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			// 恢复游戏暂停状态（核心修复！之前缺失这行导致崩溃）
			UGameplayStatics::SetGamePaused(World, false);

			// 恢复输入模式
			FInputModeGameAndUI InputModeGame;
			PC->SetInputMode(InputModeGame);

			// 隐藏鼠标并关闭交互
			PC->bShowMouseCursor = false;
			PC->bEnableClickEvents = false;
			PC->bEnableMouseOverEvents = false;

			// 确保玩家控制器重新获得焦点
			PC->SetPause(false);
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

