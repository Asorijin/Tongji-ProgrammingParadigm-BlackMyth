// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolHp.h"
#include "EventCenter.generated.h"

USTRUCT()
struct FToolsNumber {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int32 hpTools;
};
/**
 * 
 */
UCLASS()
class BLACK_MONEY_API UEventCenter : public UObject
{
	GENERATED_BODY()
private:
	void GenerateMonster();
	FVector pawnLastLocation;
	FToolsNumber* toolsNumber;
public:
	

	UEventCenter();
	/// <summary>
	/// 在调用该函数后，在角色的BeginPlay函数中设置当前位置为SpawnLocation
	/// </summary>
	/// <param name="LevelName">移动至该关卡</param>
	/// <param name="SpawnLocation">记录出生点位置</param>
	void SwitchToLevel(const FString& LevelName, FVector SpawnLocation);
	/// <summary>
	/// 获取出生点位置
	/// </summary>
	/// <returns></returns>
	const FVector GetSpawnLocation();
	/// <summary>
	/// 造成伤害
	/// </summary>
	/// <param name="makeDamager">进行攻击行为的对象</param>
	/// <param name="takeDamager">受伤对象</param>
	/// <param name="damageNumber">伤害数值</param>
	void MakeDamage(TSubclassOf<AActor> makeDamager, TSubclassOf<AActor> takeDamager, int damageNumber);
	/// <summary>
	/// 使用道具
	/// </summary>
	/// <param name="attributeVector">道具属性</param>
	void UseTools(TArray<int>* attributeVector);
	/// <summary>
	/// 拾取道具
	/// </summary>
	/// <param name="tool"></param>
	/// <param name="toolNumber"></param>
	void GetTools(AActor* tool,int toolNumber);
	void ChangeEquipment();
};
