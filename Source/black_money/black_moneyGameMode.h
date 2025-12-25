// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StartUpMenu.h"
#include "FUIPanelManager.h"
#include "GameFramework/GameModeBase.h"
#include "black_moneyGameMode.generated.h"

UCLASS(minimalapi)
class Ablack_moneyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	Ablack_moneyGameMode();
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// 新增：显示状态栏UI的函数（供启动菜单关闭时调用）
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowStatusBar();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UStartUpMenu> StartUpMenuClass;

	UUserWidget* StatusBarIns;    // 新增：角色状态栏UI

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* DefaultMusic;

	// 是否只对玩家生效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	bool bOnlyAffectPlayer = true;


	// 切换暂停状态的函数
	void TogglePause();
	// 显示/隐藏暂停UI
	void ShowPauseMenu(bool bShow);

protected:
	virtual void BeginPlay() override;

private:
	UUserWidget* StartUpMenuIns;
	
	UAudioComponent* CurrentMusicComponent;


	// 暂停UI实例
	UUserWidget* PauseMenuIns;
	// 记录当前是否暂停
	bool bIsPaused;
};



