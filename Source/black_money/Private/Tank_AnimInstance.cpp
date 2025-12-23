// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank_AnimInstance.h"
#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTank_AnimInstance::NativeInitializeAnimation()
{
	// 确保调用的是父类 UAnimInstance 的 NativeInitializeAnimation，而不是 UObject
	Super::NativeInitializeAnimation();

	APawn* OwningPawn = TryGetPawnOwner();
	TankEnemy = Cast<ABaseEnemy>(OwningPawn);

	if (TankEnemy)
	{
		TankMovement = TankEnemy->GetCharacterMovement();
	}
	else
	{
		TankMovement = nullptr;
	}

	Speed = 0.0f;
	
}
void UTank_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!TankEnemy)
	{
		APawn* OwningPawn = TryGetPawnOwner();
		TankEnemy = Cast<ABaseEnemy>(OwningPawn);

		if (TankEnemy)
		{
			TankMovement = TankEnemy->GetCharacterMovement();
		}
	}

	if (!TankEnemy || !TankMovement)
	{
		Speed = 0.0f;
		return;
	}

	// 只计算平面速度
	const FVector Velocity = TankMovement->Velocity;
	Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	// 攻击标志：BaseEnemy 里有 bIsAttacking
	bIsAttacking = TankEnemy->IsAttacking();
	
	// 闪避标志：BaseEnemy 里有 bIsDodging
	bIsDodging = TankEnemy->IsDodging();

	// 受击状态：Hit / Normal / Invulnerable
	bIsHit = (TankEnemy->GetHitState() == EEnemyHitState::Hit);

	// 死亡状态
	bIsDead = TankEnemy->IsDead();

}

