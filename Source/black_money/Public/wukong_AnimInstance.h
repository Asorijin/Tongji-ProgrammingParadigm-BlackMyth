// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "wukong_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API Uwukong_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//设置初始化动画
	virtual void NativeInitializeAnimation() override;
	//设置更新动画
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	//角色指针
	class Ablack_moneyCharacter* wukongCharacter;
	//角色移动组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* wukongMovement;
	//角色速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;
};
