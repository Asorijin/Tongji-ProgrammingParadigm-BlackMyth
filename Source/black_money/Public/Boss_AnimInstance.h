// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Boss_AnimInstance.generated.h"

class ABossEnemy;
class UCharacterMovementComponent;

/**
 * Boss动画状态枚举
 * 用于动画蓝图的Switch节点，按优先级排序
 */
UENUM(BlueprintType)
enum class EBossAnimationState : uint8
{
	Default         = 0,    // 基础状态（Idle/Walk）
	Phase1Attack    = 1,    // Phase1攻击
	Phase2Attack    = 2,    // Phase2攻击
	SkillWindup     = 3,    // 技能前摇
	SkillAttack     = 4,    // 技能攻击
	Hit             = 5,    // 受击
	PhaseTransition = 6,    // 阶段转换（咆哮）
	Death           = 7     // 死亡
};

/**
 * Boss动画实例类
 * 用于管理Boss相关的动画状态，包括阶段、怒气值、技能状态等
 */
UCLASS()
class BLACK_MONEY_API UBoss_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 初始化动画实例
	virtual void NativeInitializeAnimation() override;

	// 每帧更新动画变量
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

	// 是否正在第一阶段攻击
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsPhase1Attacking;

	// 是否正在第二阶段攻击
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsPhase2Attacking;

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

	// 当前动画状态（用于动画蓝图的Switch节点）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	EBossAnimationState CurrentAnimationState = EBossAnimationState::Default;

	// 当前动画状态（整数形式，用于动画蓝图的整数比较，0=Default, 1=Phase1Attack, 2=Phase2Attack, 3=SkillWindup, 4=SkillAttack, 5=Hit, 6=PhaseTransition, 7=Death）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	int32 CurrentAnimationStateInt = 0;

	// 获取当前动画状态（根据优先级判断）
	UFUNCTION(BlueprintCallable, Category = "Boss Animation")
	EBossAnimationState GetCurrentAnimationState() const;
};
