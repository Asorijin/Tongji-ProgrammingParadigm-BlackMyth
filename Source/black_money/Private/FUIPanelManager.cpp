#include "FUIPanelManager.h"
#include "CoreMinimal.h"
#include "UIBasePanel.h" 
TMap<UClass*, UUIBasePanel*> FUIPanelManager::PanelInstances;
UUIBasePanel* FUIPanelManager::GetOrCreatePanel(UWorld* World, TSubclassOf<UUIBasePanel> PanelClass)
{
    if (!World || !PanelClass)
        return nullptr;

    UClass* Class = PanelClass.Get();
    UUIBasePanel*& CachedInstance = PanelInstances.FindOrAdd(Class);

    if (!CachedInstance)
    {
        CachedInstance = NewObject<UUIBasePanel>(GetTransientPackage(), Class);
        CachedInstance->AddToRoot(); 
    }

    return CachedInstance;
}

void FUIPanelManager::ShowPanel(UWorld* World, TSubclassOf<UUIBasePanel> PanelClass)
{
    if (UUIBasePanel* Panel = GetOrCreatePanel(World, PanelClass))
    {
        Panel->AddToViewport();
        Panel->SetVisibility(ESlateVisibility::Visible);
        // 如果你有 NativeOnShow 虚函数，可以在这里调用
    }
}

void FUIPanelManager::HidePanel(TSubclassOf<UUIBasePanel> PanelClass)
{
    if (!PanelClass) return;

    UClass* Class = PanelClass.Get();
    if (UUIBasePanel** Found = PanelInstances.Find(Class))
    {
        if (*Found)
        {
            (*Found)->SetVisibility(ESlateVisibility::Collapsed);
            (*Found)->RemoveFromParent();
            // (*Found)->NativeOnHide();
        }

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