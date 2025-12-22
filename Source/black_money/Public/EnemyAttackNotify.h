// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyAttackNotify.generated.h"

/**
 * 怪物攻击动画通知
 * 在攻击动画的特定帧触发攻击判定
 */
UCLASS()
class BLACK_MONEY_API UEnemyAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UEnemyAttackNotify();

	// 重写通知触发方法
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// 编辑器显示名称
	virtual FString GetNotifyName_Implementation() const override;
};

