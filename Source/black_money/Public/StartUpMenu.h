// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIBasePanel.h"
#include "StartUpMenu.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API UStartUpMenu : public UUIBasePanel
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartButtonClicked();

	// 新增：Quit按钮点击函数
	UFUNCTION()
	void QuitButtonClicked();

	// 新增：设置按钮点击函数声明
	UFUNCTION()
	void SettingButtonClicked();
};
