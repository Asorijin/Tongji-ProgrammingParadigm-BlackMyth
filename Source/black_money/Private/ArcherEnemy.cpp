// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcherEnemy.h"
#include "EnemyConfig.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AArcherEnemy::AArcherEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 弓箭手初始化：设置移动速度
	// 弓箭手移动速度较慢，适合远程单位
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// 设置攻击范围：弓箭手的攻击范围很大，适合远程攻击
	if (AttackRangeSphere)
	{
		AttackRangeSphere->SetSphereRadius(1000.0f);
	}

	// 设置检测范围：弓箭手能较早发现玩家
	if (DetectionSphere)
	{
		DetectionSphere->SetSphereRadius(1200.0f);
	}

	// 配置闪避系统参数：弓箭手闪避能力较强
	DodgeProbability = 0.7f;      // 70%闪避概率
	DodgeRange = 400.0f;           // 检测玩家攻击的范围
	DodgeStrength = 1000.0f;       // 闪避时的移动力度
	DodgeDuration = 0.25f;         // 闪避动画持续时间
	DodgeCooldown = 1.5f;          // 闪避冷却时间
}

void AArcherEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 配置弓箭手的 EnemyConfig 属性
	if (EnemyConfig)
	{
		// 弓箭手属性：低血量、中等攻击、低防御
		EnemyConfig->MaxHp = 60;
		EnemyConfig->CurrentHp = 60;
		EnemyConfig->Attack = 15;
		EnemyConfig->Defence = 2;

		// 配置移动速度和攻击速度，以及攻击范围
		EnemyConfig->MoveSpeed = 200.0f;
		EnemyConfig->AttackSpeed = 0.8f;      // 每秒1.25次攻击
		EnemyConfig->AttackRange = 1000.0f;    // 攻击范围很大，适合远程单位
		EnemyConfig->DetectionRange = 1200.0f; // 检测范围

		// 初始化 EnemyConfig，确保当前血量不超过最大血量
		EnemyConfig->Initialize();
	}

	// 同步移动速度到角色移动组件
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = EnemyConfig ? EnemyConfig->MoveSpeed : 200.0f;
	}
}

