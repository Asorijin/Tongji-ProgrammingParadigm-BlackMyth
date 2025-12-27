#include "FUIPanelManager.h"
#include "CoreMinimal.h"
#include "UIBasePanel.h" 
TMap<FString, UUIBasePanel*> FUIPanelManager::PanelInstances;
UUIBasePanel* FUIPanelManager::GetOrCreatePanel(UWorld* World, TSubclassOf<UUIBasePanel> PanelClass,FString ClassName)
{
    if (!World || !PanelClass)
        return nullptr;

    UClass* Class = PanelClass.Get();
    UUIBasePanel*& CachedInstance = PanelInstances.FindOrAdd(ClassName);

    if (!CachedInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Creating NEW panel: %s"), *ClassName);
        CachedInstance = CreateWidget<UUIBasePanel>(World, Class);
        if (CachedInstance)
        {
            CachedInstance->AddToRoot();
        }
    }

    return CachedInstance;
}

void FUIPanelManager::ShowPanel(UWorld* World, TSubclassOf<UUIBasePanel> PanelClass,FString ClassName)
{
    if (UUIBasePanel* Panel = GetOrCreatePanel(World, PanelClass,ClassName))
    {
        Panel->AddToViewport();
        Panel->SetVisibility(ESlateVisibility::Visible);
        // 如果你有 NativeOnShow 虚函数，可以在这里调用
    }
}

void FUIPanelManager::HidePanel(FString ClassName)
{
    UE_LOG(LogTemp, Warning, TEXT("Panel Now Num: %d"), PanelInstances.Num());
    if (UUIBasePanel** Found = PanelInstances.Find(ClassName))
    {
        if (*Found)
        {
            UE_LOG(LogTemp, Warning, TEXT("Hiding panel: %s"), *ClassName);
            (*Found)->SetVisibility(ESlateVisibility::Collapsed);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Not Hide panel: %s"), *ClassName);
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Not Find panel: %s"), *ClassName);
    }
}

void FUIPanelManager::ClearAllPanels()
{
    for (auto& Pair : PanelInstances)
    {
        if (Pair.Value)
        {
            Pair.Value->RemoveFromRoot();
        }
    }
    PanelInstances.Empty();
}