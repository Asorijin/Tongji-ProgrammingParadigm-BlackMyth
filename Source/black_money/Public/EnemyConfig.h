// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyConfig.generated.h"

/**
 * 怪物配置类
 * 用于管理怪物的属性数据（HP、攻击力、防御力等）
 * 继承自UObject，作为数据资产使用
 */
UCLASS()
class BLACK_MONEY_API UEnemyConfig : public UObject
{
	GENERATED_BODY()

public:
	UEnemyConfig();

	// 最大生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int32 MaxHp = 100;

	// 当前生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int32 CurrentHp = 100;

	// 攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int32 Attack = 10;

	// 防御力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	int32 Defence = 5;

	// 移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float MoveSpeed = 150.0f;

	// 攻击速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float AttackSpeed = 1.0f;

	// 攻击范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float AttackRange = 150.0f;

	// 检测范围（发现玩家的距离）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float DetectionRange = 1000.0f;

	// 初始化配置（设置默认值）
	void Initialize();

	// 获取生命值百分比
	float GetHealthPercentage() const;

	// 检查是否死亡
	bool IsDead() const;
};

