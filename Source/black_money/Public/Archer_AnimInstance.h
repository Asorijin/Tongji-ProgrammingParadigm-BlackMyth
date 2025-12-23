// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Archer_AnimInstance.generated.h"
class ABaseEnemy;
class UCharacterMovementComponent;
/**
 * 弓箭手动画实例类
 * 用于管理弓箭手怪物的动画状态
 */
UCLASS()
class BLACK_MONEY_API UArcher_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 初始化动画实例
	virtual void NativeInitializeAnimation() override;

	// 每帧更新动画数据
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 敌人角色指针
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	ABaseEnemy* ArcherEnemy;

	// 敌人移动组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* ArcherMovement;

	// 敌人速度
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
	
};

