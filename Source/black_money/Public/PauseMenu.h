// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIBasePanel.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API UPauseMenu : public UUIBasePanel
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ContinueButtonClicked();

	// 新增：Quit按钮点击函数
	UFUNCTION()
	void QuitButtonClicked();

	// 新增：设置按钮点击函数声明
	UFUNCTION()
	void SettingButtonClicked();
};
