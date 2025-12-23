// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "AudioDevice.h"
#include "Engine/PostProcessVolume.h"

void USettingMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UButton* SetQuitButton = CastChecked<UButton>(GetWidgetFromName(FName("Btn_SetQuit"))))
	{
		FScriptDelegate SetQuitDelegate;
		SetQuitDelegate.BindUFunction(this, "QuitButtonClicked");
		SetQuitButton->OnClicked.Add(SetQuitDelegate);
	}

    if (UTextBlock* VolText = Cast<UTextBlock>(GetWidgetFromName(FName("Text_Volume"))))
    {
        VolumeText = VolText;
    }

    // 绑定音量滑块（需与蓝图中滑块的名称一致，假设蓝图中名称为"Slider_Volume"）
    if (USlider* VolSlider = Cast<USlider>(GetWidgetFromName(FName("Slider_Volume"))))
    {
        VolumeSlider = VolSlider;
        FScriptDelegate VolumeDelegate;
        VolumeDelegate.BindUFunction(this, "OnVolumeSliderChanged");
        VolumeSlider->OnValueChanged.Add(VolumeDelegate);
    }

    // 绑定亮度滑块（需与蓝图中滑块的名称一致，假设蓝图中名称为"Slider_Brightness"）
    if (USlider* BrightSlider = Cast<USlider>(GetWidgetFromName(FName("Slider_Brightness"))))
    {
        BrightnessSlider = BrightSlider;
        FScriptDelegate BrightnessDelegate;
        BrightnessDelegate.BindUFunction(this, "OnBrightnessSliderChanged");
        BrightnessSlider->OnValueChanged.Add(BrightnessDelegate);
    }

    if (UTextBlock* BrightText = Cast<UTextBlock>(GetWidgetFromName(FName("Text_Brightness"))))
    {
        BrightnessText = BrightText;
    }
}

void USettingMenu::QuitButtonClicked()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Quit"));
	RemoveFromViewport();
}

// 处理音量滑块值变化
void USettingMenu::OnVolumeSliderChanged(float NewValue)
{
    if (VolumeText)
    {
        // 将滑块值（0.0~1.0）转换为百分比文本（0%~100%）
        FString VolumeStr = FString::Printf(TEXT("%.0f%%"), NewValue);
        VolumeText->SetText(FText::FromString(VolumeStr));
    }
}

// 处理亮度滑块值变化
void USettingMenu::OnBrightnessSliderChanged(float NewValue)
{
    if (BrightnessText)
    {
        // 将滑块值（0.0~1.0）转换为百分比文本（0%~100%）
        FString BrightnessStr = FString::Printf(TEXT("%.0f%%"), NewValue * 100);
        BrightnessText->SetText(FText::FromString(BrightnessStr));
    }
}