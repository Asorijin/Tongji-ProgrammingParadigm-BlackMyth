// Fill out your copyright notice in the Description page of Project Settings.


#include "Mutant_AnimInstance.h"
#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMutant_AnimInstance::NativeInitializeAnimation()
{
	// 确保调用的是父类 UAnimInstance 的 NativeInitializeAnimation，而不是 UObject
	Super::NativeInitializeAnimation();

	APawn* OwningPawn = TryGetPawnOwner();
	MutantEnemy = Cast<ABaseEnemy>(OwningPawn);

	if (MutantEnemy)
	{
		MutantMovement = MutantEnemy->GetCharacterMovement();
	}
	else
	{
		MutantMovement = nullptr;
	}

	Speed = 0.0f;
	
}
void UMutant_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!MutantEnemy)
	{
		APawn* OwningPawn = TryGetPawnOwner();
		MutantEnemy = Cast<ABaseEnemy>(OwningPawn);

		if (MutantEnemy)
		{
			MutantMovement = MutantEnemy->GetCharacterMovement();
		}
	}

	if (!MutantEnemy || !MutantMovement)
	{
		Speed = 0.0f;
		return;
	}

	// 只计算平面速度
	const FVector Velocity = MutantMovement->Velocity;
	Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	// 攻击标志：BaseEnemy 里有 bIsAttacking，给它加一个 Getter（见下方）
	bIsAttacking = MutantEnemy->IsAttacking();

	// 受击状态：Hit / Normal / Invulnerable
 
	bIsHit = (MutantEnemy->GetHitState() == EEnemyHitState::Hit);

	// 死亡状态
	bIsDead = MutantEnemy->IsDead();

}