// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	UUserWidget* StatusBarIns;    // 新增：角色状态栏UI

protected:
	virtual void BeginPlay() override;

private:
	UUserWidget* StartUpMenuIns;
	
};



