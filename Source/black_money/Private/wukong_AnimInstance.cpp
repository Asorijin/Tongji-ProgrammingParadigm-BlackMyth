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
	if(wukongCharacter&&wukongMovement)
	{
		//获取角色速度
		Speed = wukongMovement->Velocity.Size();
	}
}
