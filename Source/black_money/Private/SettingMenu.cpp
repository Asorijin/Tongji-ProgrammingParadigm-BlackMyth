// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/PostProcessVolume.h"
#include "FUIPanelManager.h"
#include "AudioDeviceManager.h"
#include "AudioDevice.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Sound/SoundClass.h"
#include "black_moneyGameInstance.h"

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

    // 从GameInstance加载保存的设置
    LoadSettingsFromGameInstance();

    InitializeBrightness();
}

// 从GameInstance加载设置
void USettingMenu::LoadSettingsFromGameInstance()
{
    if (Ublack_moneyGameInstance* GameInstance = Cast<Ublack_moneyGameInstance>(GetGameInstance()))
    {
        // 加载音量设置
        float SavedVolume = GameInstance->GetSavedVolume();
        if (VolumeSlider)
        {
            VolumeSlider->SetValue(SavedVolume);
            OnVolumeSliderChanged(SavedVolume); // 更新显示
        }

        // 加载亮度设置
        float SavedBrightness = GameInstance->GetSavedBrightness();
        if (BrightnessSlider)
        {
            BrightnessSlider->SetValue(SavedBrightness);
            OnBrightnessSliderChanged(SavedBrightness); // 更新显示
        }
    }
}

void USettingMenu::QuitButtonClicked()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    FUIPanelManager::HidePanel(FString("SettingWidget"));
    FUIPanelManager::HidePanel(FString("PSettingWidget"));
}

// 处理音量滑块值变化
void USettingMenu::OnVolumeSliderChanged(float NewValue)
{
    if (VolumeText)
    {
        FString VolumeStr = FString::Printf(TEXT("%.0f%%"), NewValue);
        VolumeText->SetText(FText::FromString(VolumeStr));
    }

    // 保存到GameInstance（新增代码）
    if (Ublack_moneyGameInstance* GameInstance = Cast<Ublack_moneyGameInstance>(GetGameInstance()))
    {
        GameInstance->SetSavedVolume(NewValue);
    }

    float VolumeLevel = NewValue / 100.0f; // 转换为0-1范围
    if (USoundClass* MasterSoundClass = LoadObject<USoundClass>(nullptr, TEXT("/Engine/EngineSounds/Master.Master")))
    {
        MasterSoundClass->Properties.Volume = VolumeLevel;
    }
}

// 处理亮度滑块值变化
void USettingMenu::OnBrightnessSliderChanged(float NewValue)
{
    if (BrightnessText)
    {
        FString BrightnessStr = FString::Printf(TEXT("%.0f%%"), NewValue);
        BrightnessText->SetText(FText::FromString(BrightnessStr));
    }

    // 保存到GameInstance（新增代码）
    if (Ublack_moneyGameInstance* GameInstance = Cast<Ublack_moneyGameInstance>(GetGameInstance()))
    {
        GameInstance->SetSavedBrightness(NewValue);
    }

    // 实际应用亮度到引擎（核心补充）
    SetDisplayGamma(NewValue);
}


// 初始化亮度值（类似GammaWidget的Initialize）
void USettingMenu::InitializeBrightness()
{
    if (GEngine)
    {
        // 从引擎获取当前伽马值并转换为滑块范围(0-100)
        CurrentBrightness = (GEngine->DisplayGamma - 1.0f) / 4.0f * 100.0f;

        // 同步滑块位置（如果亮度滑块存在）
        if (BrightnessSlider)
        {
            BrightnessSlider->SetValue(CurrentBrightness);
            OnBrightnessSliderChanged(CurrentBrightness);
        }
    }
}

// 设置显示伽马值（核心功能，类似GammaWidget的SetDisplayGamma）
void USettingMenu::SetDisplayGamma(float InGamma)
{
    CurrentBrightness = InGamma;
    if (GEngine)
    {
        // 将0-100的滑块值转换为引擎伽马值范围(1.0-5.0)
        GEngine->DisplayGamma = 1.0f + (InGamma / 100.0f) * 4.0f;
    }
}

// 获取当前亮度值（类似GammaWidget的GetDisplayGamma）
float USettingMenu::GetDisplayGamma()
{
    return CurrentBrightness;
}