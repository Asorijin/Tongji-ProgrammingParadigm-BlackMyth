// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBasePanel.generated.h"

/**
 * UI面板类继承该类，进行实现
 */
UCLASS(Blueprintable, BlueprintType)
class BLACK_MONEY_API UUIBasePanel : public UUserWidget
{
	GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void ShowPanel()
    {
        AddToViewport();
        SetVisibility(ESlateVisibility::Visible);
        NativeOnShow();
    }

    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void HidePanel()
    {
        SetVisibility(ESlateVisibility::Collapsed);
        RemoveFromParent();
        NativeOnHide();
    }

protected:
    virtual void NativeOnShow() {}
    virtual void NativeOnHide() {}
};
