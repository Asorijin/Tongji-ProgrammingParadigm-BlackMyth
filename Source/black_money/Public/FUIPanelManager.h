// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Engine/World.h"
#include "UObject/Class.h"
#include "Templates/SubclassOf.h"
#include "UIBasePanel.h"

class UUIBasePanel;

/**
 * UI √Ê∞Âπ‹¿Ì∆˜
 */
class BLACK_MONEY_API FUIPanelManager
{
public:
    static UUIBasePanel* GetOrCreatePanel(UWorld* World, TSubclassOf<UUIBasePanel> PanelClass, FString ClassName);

    static void ShowPanel(UWorld* World, TSubclassOf<UUIBasePanel> PanelClass, FString ClassName);
    static void HidePanel(FString ClassName);

    static void ClearAllPanels();

private:
    static TMap<FString, UUIBasePanel*> PanelInstances;
};