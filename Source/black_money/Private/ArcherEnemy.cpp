// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcherEnemy.h"
#include "EnemyConfig.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AArcherEnemy::AArcherEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 弓箭手：远程输出型，高机动性
	// 设置移动速度（比基础怪物更快）
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// 设置攻击范围检测组件半径（远程攻击，范围更大）
	if (AttackRangeSphere)
	{
		AttackRangeSphere->SetSphereRadius(500.0f);
	}

	// 设置检测范围组件半径（更远的检测范围）
	if (DetectionSphere)
	{
		DetectionSphere->SetSphereRadius(1200.0f);
	}

	// 设置闪避系统默认属性（高机动性）
	DodgeProbability = 0.7f;      // 70%闪避概率
	DodgeRange = 400.0f;           // 更大的闪避检测范围
	DodgeStrength = 1000.0f;       // 更强的闪避移动速度
	DodgeDuration = 0.25f;         // 更短的闪避持续时间
	DodgeCooldown = 1.5f;          // 更短的冷却时间
}

void AArcherEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 配置弓箭手的EnemyConfig属性
	if (EnemyConfig)
	{
		// 基础属性：低血量、低防御、高攻击
		EnemyConfig->MaxHp = 60;
		EnemyConfig->CurrentHp = 60;
		EnemyConfig->Attack = 15;
		EnemyConfig->Defence = 2;

		// 移动和攻击属性：高移动速度、高攻击速度、远程攻击范围
		EnemyConfig->MoveSpeed = 200.0f;
		EnemyConfig->AttackSpeed = 0.8f;      // 每秒约1.25次攻击
		EnemyConfig->AttackRange = 500.0f;     // 远程攻击范围
		EnemyConfig->DetectionRange = 1200.0f; // 更远的检测范围

		// 确保当前生命值不超过最大生命值
		EnemyConfig->Initialize();
	}

	// 同步移动速度到角色移动组件
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = EnemyConfig ? EnemyConfig->MoveSpeed : 200.0f;
	}
}

