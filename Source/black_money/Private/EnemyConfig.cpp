// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyConfig.h"

UEnemyConfig::UEnemyConfig()
{
	// 初始化默认值
	MaxHp = 100;
	CurrentHp = 100;
	Attack = 10;
	Defence = 5;
	MoveSpeed = 300.0f;
	AttackSpeed = 1.0f;
	AttackRange = 150.0f;
	DetectionRange = 1000.0f;
}

void UEnemyConfig::Initialize()
{
	// 确保当前生命值不超过最大生命值
	CurrentHp = FMath::Clamp(CurrentHp, 0, MaxHp);
}

float UEnemyConfig::GetHealthPercentage() const
{
	if (MaxHp <= 0)
	{
		return 0.0f;
	}
	return static_cast<float>(CurrentHp) / static_cast<float>(MaxHp);
}

bool UEnemyConfig::IsDead() const
{
	return CurrentHp <= 0;
}

