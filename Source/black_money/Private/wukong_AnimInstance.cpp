// Fill out your copyright notice in the Description page of Project Settings.


#include "wukong_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "black_money/black_moneyCharacter.h"
void Uwukong_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//获取角色指针，并进行类型转换
	wukongCharacter = Cast<Ablack_moneyCharacter>(TryGetPawnOwner());
	if (wukongCharacter)
	{
		//获取角色移动组件
		wukongMovement=wukongCharacter->GetCharacterMovement();
	}
}

void Uwukong_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 防止未初始化读取脏值
	Speed = 0.0f;
	bIsDodging = false;

	// 确保角色指针和移动组件存在（允许在运行时补充）
	if (!wukongCharacter)
	{
		wukongCharacter = Cast<Ablack_moneyCharacter>(TryGetPawnOwner());
		if (wukongCharacter)
		{
			wukongMovement = wukongCharacter->GetCharacterMovement();
		}
	}

	if (wukongCharacter && wukongMovement)
	{
		// 水平速度（忽略 Z）
		const FVector Velocity = wukongMovement->Velocity;
		const FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.0f);

		// 速度大小（用于 速度判断）
		Speed = HorizontalVelocity.Size();

		// 是否在空中（跳跃/下落）
		bIsInAir = wukongMovement->IsFalling();
		// 判断是否在跳跃最高点（上升到下降的过渡）
		bIsAtJumpApex = bIsInAir && (wukongMovement->Velocity.Z <= 0.0f);
		
		// 从角色获取闪避状态
		bIsDodging = wukongCharacter->IsDodging();
		//从角色获取攻击状态
		bIsAttacking = wukongCharacter->IsAttacking();
	}
}
