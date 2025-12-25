// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Boss_AnimInstance.generated.h"

class ABossEnemy;
class UCharacterMovementComponent;

/**
 * Boss动画实例类
 * 用于管理Boss怪物的动画状态，包括阶段、怒气值、技能状态等
 */
UCLASS()
class BLACK_MONEY_API UBoss_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 初始化动画实例
	virtual void NativeInitializeAnimation() override;

	// 每帧更新动画数据
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Boss角色指针
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	ABossEnemy* BossEnemy;

	// Boss移动组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* BossMovement;

	// Boss速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;

	// 是否正在攻击
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking;

	// 是否正在闪避
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsDodging;

	// 是否处于受击硬直/受击状态
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsHit;

	// 是否死亡
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsDead;

	// 当前阶段（用于动画蓝图）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	int32 CurrentPhase; // 0 = Phase1, 1 = Phase2, 2 = PhaseTransition

	// 是否正在使用技能
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	bool bIsUsingSkill;

	// 怒气值百分比（0.0 - 1.0）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	float RagePercentage;
};

