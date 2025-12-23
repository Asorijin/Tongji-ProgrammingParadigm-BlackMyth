// Fill out your copyright notice in the Description page of Project Settings.


#include "Archer_AnimInstance.h"
#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UArcher_AnimInstance::NativeInitializeAnimation()
{
	// 确保调用的是父类 UAnimInstance 的 NativeInitializeAnimation，而不是 UObject
	Super::NativeInitializeAnimation();

	APawn* OwningPawn = TryGetPawnOwner();
	ArcherEnemy = Cast<ABaseEnemy>(OwningPawn);

	if (ArcherEnemy)
	{
		ArcherMovement = ArcherEnemy->GetCharacterMovement();
	}
	else
	{
		ArcherMovement = nullptr;
	}

	Speed = 0.0f;
	
}
void UArcher_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!ArcherEnemy)
	{
		APawn* OwningPawn = TryGetPawnOwner();
		ArcherEnemy = Cast<ABaseEnemy>(OwningPawn);

		if (ArcherEnemy)
		{
			ArcherMovement = ArcherEnemy->GetCharacterMovement();
		}
	}

	if (!ArcherEnemy || !ArcherMovement)
	{
		Speed = 0.0f;
		return;
	}

	// 只计算平面速度
	const FVector Velocity = ArcherMovement->Velocity;
	Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	// 攻击标志：BaseEnemy 里有 bIsAttacking
	bIsAttacking = ArcherEnemy->IsAttacking();
	
	// 闪避标志：BaseEnemy 里有 bIsDodging
	bIsDodging = ArcherEnemy->IsDodging();

	// 受击状态：Hit / Normal / Invulnerable
	bIsHit = (ArcherEnemy->GetHitState() == EEnemyHitState::Hit);

	// 死亡状态
	bIsDead = ArcherEnemy->IsDead();

}

