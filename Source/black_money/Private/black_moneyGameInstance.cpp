// Fill out your copyright notice in the Description page of Project Settings.

#include "black_moneyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "FUIPanelManager.h"
#include "CharacterConfig.h"

Ublack_moneyGameInstance::Ublack_moneyGameInstance()
{
    // …Ë÷√ƒ¨»œ÷µ£®0-100∑∂Œß£©
    SavedVolume = 100.0f;
    SavedBrightness = 100.0f;
}

void Ublack_moneyGameInstance::Init() {
    UGameInstance::Init();

    eventCenter = NewObject<UEventCenter>(this);

    eventCenter->ReadLastState();

    bIsFirstLaunch = true;
}
void Ublack_moneyGameInstance::Shutdown() {
    FUIPanelManager::ClearAllPanels();
    UGameInstance::Shutdown();
}

void Ublack_moneyGameInstance::SetSavedVolume(float Volume)
{
    SavedVolume = FMath::Clamp(Volume, 0.0f, 100.0f);
}

void Ublack_moneyGameInstance::SetSavedBrightness(float Brightness)
{
    SavedBrightness = FMath::Clamp(Brightness, 0.0f, 100.0f);
}