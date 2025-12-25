// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Engine/PostProcessVolume.h"
#include "FUIPanelManager.h"
#include "AudioMixerBlueprintLibrary.h"

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

    /*需要实现音量控制，以下代码错误*/
    //UAudioMixerBlueprintLibrary::SetMasterOutputVolume(GetWorld(), NewValue);
}

// 处理亮度滑块值变化
void USettingMenu::OnBrightnessSliderChanged(float NewValue)
{
    if (BrightnessText)
    {
        FString BrightnessStr = FString::Printf(TEXT("%.0f%%"), NewValue);
        BrightnessText->SetText(FText::FromString(BrightnessStr));
    }

    /*需要实现亮度控制，以下代码错误*/
    //TArray<AActor*> PostProcessVolumes;
    //UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), PostProcessVolumes);
    //for (AActor* Actor : PostProcessVolumes)
    //{
    //    APostProcessVolume* PPVolume = Cast<APostProcessVolume>(Actor);
    //    if (PPVolume && PPVolume->bUnbound)
    //    {
    //        float ExposureValue = FMath::Lerp(-4.0f, 4.0f, NewValue);
    //        // 修复7: 正确应用后处理设置（必须复制-修改-赋值）
    //        FPostProcessSettings NewSettings = PPVolume->Settings;
    //        NewSettings.Exposure.Compensation = ExposureValue;
    //        NewSettings.Exposure.bOverride_Compensation = true;
    //        PPVolume->Settings = NewSettings;
    //        break;
    //    }
    //}
}