// Fill out your copyright notice in the Description page of Project Settings.

#include "TankEnemy.h"
#include "EnemyConfig.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATankEnemy::ATankEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 坦克：高防御高血量型，低机动性
	// 设置移动速度（比基础怪物更慢）
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	// 设置攻击范围检测组件半径（近战攻击，范围较小）
	if (AttackRangeSphere)
	{
		AttackRangeSphere->SetSphereRadius(120.0f);
	}

	// 设置检测范围组件半径（较短的检测范围）
	if (DetectionSphere)
	{
		DetectionSphere->SetSphereRadius(800.0f);
	}

	// 设置闪避系统默认属性（低机动性）
	DodgeProbability = 0.2f;      // 20%闪避概率
	DodgeRange = 200.0f;         // 较小的闪避检测范围
	DodgeStrength = 500.0f;       // 较弱的闪避移动速度
	DodgeDuration = 0.4f;        // 较长的闪避持续时间
	DodgeCooldown = 3.0f;        // 较长的冷却时间
}

void ATankEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 配置坦克的EnemyConfig属性
	if (EnemyConfig)
	{
		// 基础属性：高防御、低攻击（调整为8次攻击死亡）
		EnemyConfig->MaxHp = 120;      // 120 ÷ 15 = 8次攻击死亡
		EnemyConfig->CurrentHp = 120;
		EnemyConfig->Attack = 8;
		EnemyConfig->Defence = 15;

		// 移动和攻击属性：低移动速度、低攻击速度、近战攻击范围
		EnemyConfig->MoveSpeed = 100.0f;
		EnemyConfig->AttackSpeed = 0.6f;      // 每秒约0.6次攻击
		EnemyConfig->AttackRange = 120.0f;     // 近战攻击范围
		EnemyConfig->DetectionRange = 800.0f;  // 较短的检测范围

		// 确保当前生命值不超过最大生命值
		EnemyConfig->Initialize();
	}

	// 同步移动速度到角色移动组件
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = EnemyConfig ? EnemyConfig->MoveSpeed : 100.0f;
	}
}

