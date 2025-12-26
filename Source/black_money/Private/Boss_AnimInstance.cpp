// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_AnimInstance.h"
#include "BossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

void UBoss_AnimInstance::NativeInitializeAnimation()
{
	// 确保调用的是父类 UAnimInstance 的 NativeInitializeAnimation
	Super::NativeInitializeAnimation();

	APawn* OwningPawn = TryGetPawnOwner();
	BossEnemy = Cast<ABossEnemy>(OwningPawn);

	if (BossEnemy)
	{
		BossMovement = BossEnemy->GetCharacterMovement();
	}
	else
	{
		BossMovement = nullptr;
	}

	Speed = 0.0f;
	CurrentPhase = 0;
	bIsAttacking = false;
	bIsPhase1Attacking = false;
	bIsPhase2Attacking = false;
	bIsUsingSkill = false;
	RagePercentage = 0.0f;
	CurrentAnimationState = EBossAnimationState::Default;
	CurrentAnimationStateInt = 0;
}

void UBoss_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!BossEnemy)
	{
		APawn* OwningPawn = TryGetPawnOwner();
		BossEnemy = Cast<ABossEnemy>(OwningPawn);

		if (BossEnemy)
		{
			BossMovement = BossEnemy->GetCharacterMovement();
		}
	}

	if (!BossEnemy || !BossMovement)
	{
		Speed = 0.0f;
		return;
	}

	// 只计算平面速度
	const FVector Velocity = BossMovement->Velocity;
	Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();

	// 攻击标志
	bIsAttacking = BossEnemy->IsAttacking();

	// Boss特有状态
	EBossPhase Phase = BossEnemy->GetCurrentPhase();
	CurrentPhase = static_cast<int32>(Phase);

	// 区分两个阶段的攻击状态
	bIsPhase1Attacking = bIsAttacking && (Phase == EBossPhase::Phase1);
	bIsPhase2Attacking = bIsAttacking && (Phase == EBossPhase::Phase2);

	// 闪避标志
	bIsDodging = BossEnemy->IsDodging();

	// 受击状态
	bIsHit = (BossEnemy->GetHitState() == EEnemyHitState::Hit);

	// 死亡状态
	bIsDead = BossEnemy->IsDead();

	// Boss特有状态（阶段信息已在上面获取）
	bIsUsingSkill = BossEnemy->IsUsingSkill();
	RagePercentage = BossEnemy->GetRagePercentage();

	// 更新当前动画状态
	CurrentAnimationState = GetCurrentAnimationState();
	CurrentAnimationStateInt = static_cast<int32>(CurrentAnimationState);
}

EBossAnimationState UBoss_AnimInstance::GetCurrentAnimationState() const
{
	if (!BossEnemy)
	{
		return EBossAnimationState::Default;
	}

	// 1. 死亡（最高优先级）
	if (BossEnemy->IsDead())
	{
		return EBossAnimationState::Death;
	}

	// 2. 阶段转换（第二优先级）
	EBossPhase Phase = BossEnemy->GetCurrentPhase();
	if (Phase == EBossPhase::PhaseTransition)
	{
		return EBossAnimationState::PhaseTransition;
	}

	// 3. 受击（第三优先级）
	if (BossEnemy->GetHitState() == EEnemyHitState::Hit)
	{
		return EBossAnimationState::Hit;
	}

	// 4. 技能状态（需要区分前摇和攻击）
	if (BossEnemy->IsUsingSkill())
	{
		// 获取技能蒙太奇引用
		UAnimMontage* SkillAttackMontage = BossEnemy->GetSkillAttackMontage();
		UAnimMontage* SkillWindupMontage = BossEnemy->GetSkillWindupMontage();

		// 检查当前正在播放的蒙太奇
		if (SkillAttackMontage && Montage_IsPlaying(SkillAttackMontage))
		{
			return EBossAnimationState::SkillAttack;
		}
		else if (SkillWindupMontage && Montage_IsPlaying(SkillWindupMontage))
		{
			return EBossAnimationState::SkillWindup;
		}
		else
		{
			// 如果无法判断，默认返回前摇（技能状态刚开始）
			return EBossAnimationState::SkillWindup;
		}
	}

	// 5. Phase2攻击
	if (Phase == EBossPhase::Phase2 && BossEnemy->IsAttacking())
	{
		return EBossAnimationState::Phase2Attack;
	}

	// 6. Phase1攻击
	if (Phase == EBossPhase::Phase1 && BossEnemy->IsAttacking())
	{
		return EBossAnimationState::Phase1Attack;
	}

	// 7. 默认状态
	return EBossAnimationState::Default;
}

