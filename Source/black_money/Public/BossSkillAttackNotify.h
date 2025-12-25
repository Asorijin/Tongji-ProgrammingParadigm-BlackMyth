// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BossSkillAttackNotify.generated.h"

/**
 * Boss技能攻击动画通知
 * 在技能攻击动画的特定帧触发伤害判定
 */
UCLASS()
class BLACK_MONEY_API UBossSkillAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UBossSkillAttackNotify();

	// 重写通知触发函数
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// 编辑器显示名称
	virtual FString GetNotifyName_Implementation() const override;
};

