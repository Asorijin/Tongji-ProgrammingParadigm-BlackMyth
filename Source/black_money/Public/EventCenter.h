// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventCenter.generated.h"

USTRUCT()
struct FToolsNumber {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int32 hpTools;
};

/**
 * 事件中心类
 * 负责管理游戏中的各种事件，实现系统间的解耦
 */
UCLASS()
class BLACK_MONEY_API UEventCenter : public UObject
{
	GENERATED_BODY()

private:
	// 生成怪物（私有方法）
	void GenerateMonster();
	
	// 角色最后位置（用于关卡切换时保存位置）
	FVector pawnLastLocation;
	
	// 角色最后关卡
	FString levelName;

	// 工具数量统计
	FToolsNumber* toolsNumber;

public:
	UEventCenter();

	/**
	 * 切换关卡
	 * 在调用该函数前，在角色的BeginPlay中调用，设置当前位置为SpawnLocation
	 * @param LevelName 要切换到的关卡名称
	 * @param SpawnLocation 记录的角色位置
	 */
	void SwitchToLevel(const FString& LevelName, FVector SpawnLocation);

	/**
	 * 获取生成位置
	 * @return 返回保存的角色位置
	 */
	const FVector GetSpawnLocation();

	/**
	 * @param makeDamager 造成伤害行为的对象类型
	 * @param takeDamager 受伤害对象类型
	 * @param damageNumber 伤害数值
	 */


	void MakeDamage(TSubclassOf<AActor*> makeDamager, TArray<TSubclassOf<AActor*>> takeDamager, int damageNumber);

	/**
	 * 使用道具
	 * @param tool 道具指针
	 */
	void UseTools(AActor* tool);

	/**
	 * 拾取道具
	 * @param tool 道具对象
	 * @param toolNumber 道具编号

	 */
	void GetTools(AActor* tool, int toolNumber);

	/**
	 * 更换装备
	 */
	void ChangeEquipment();
};
