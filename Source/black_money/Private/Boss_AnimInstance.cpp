// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss_AnimInstance.h"
#include "BossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	bIsUsingSkill = false;
	RagePercentage = 0.0f;
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

	// 闪避标志
	bIsDodging = BossEnemy->IsDodging();

	// 受击状态
	bIsHit = (BossEnemy->GetHitState() == EEnemyHitState::Hit);

	// 死亡状态
	bIsDead = BossEnemy->IsDead();

	// Boss特有状态
	EBossPhase Phase = BossEnemy->GetCurrentPhase();
	CurrentPhase = static_cast<int32>(Phase);
	bIsUsingSkill = BossEnemy->IsUsingSkill();
	RagePercentage = BossEnemy->GetRagePercentage();
}

