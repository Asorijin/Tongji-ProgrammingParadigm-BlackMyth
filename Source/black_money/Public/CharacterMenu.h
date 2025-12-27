// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIBasePanel.h"
#include "Components/TextBlock.h"
#include "black_money/black_moneyCharacter.h"
#include "CharacterMenu.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API UCharacterMenu : public UUIBasePanel
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    // 绑定UI中的进度条控件
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ManaBar; // 法力值进度条

    // 绑定技能冷却文本控件（替换原进度条）
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EarthQuakeCooldownText;

    // 新增：HP药水数量文本控件（用于在蓝图中绑定UI）
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HpPotionCountText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MpPotionCountText;

    // 玩家角色引用
    Ablack_moneyCharacter* PlayerCharacter;

    // 更新血量显示
    void UpdateHealthDisplay();

    void UpdateManaDisplay();

    void UpdateCooldownDisplay();

    // 新增：更新HP药水数量显示
    void UpdateHpPotionCountDisplay();
    void UpdateMpPotionCountDisplay();
};
