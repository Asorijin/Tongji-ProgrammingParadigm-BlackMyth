// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIBasePanel.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Engine/PostProcessVolume.h"
#include "SettingMenu.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API USettingMenu : public UUIBasePanel
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void QuitButtonClicked();

	// 声明滑块值变化的处理函数（参数为滑块当前值）
	UFUNCTION()
	void OnVolumeSliderChanged(float NewValue);

	UFUNCTION()
	void OnBrightnessSliderChanged(float NewValue);

private:
	// 滑块控件的指针（可选，用于后续访问）
	USlider* VolumeSlider;
	USlider* BrightnessSlider;

	// 文本控件指针（用于显示滑块值，新增）
	UTextBlock* VolumeText;   // 显示音量值
	UTextBlock* BrightnessText; // 显示亮度值

	// 声明加载设置的函数（新增）
	void LoadSettingsFromGameInstance();

	// 添加新变量：用于控制亮度的后处理体积
	UPROPERTY(EditAnywhere, Category = "Brightness")
	class APostProcessVolume* PostProcessVolume;
};
