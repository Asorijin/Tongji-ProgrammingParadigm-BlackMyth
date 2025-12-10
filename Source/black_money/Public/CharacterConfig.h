// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterConfig.generated.h"

USTRUCT()
struct FEquipment{

	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int32 head = 0;
	UPROPERTY()
	int32 body = 0;
	UPROPERTY()
	int32 boot = 0;
	UPROPERTY()
	int32 weapon = 0;
};

/**
 * 初步计划是Character类在初始化时使用该类读取配置文件，初始化数值
 * 游戏过程中角色的数值存储在该类中
 */
UCLASS()
class BLACK_MONEY_API UCharacterConfig : public UObject
{
	GENERATED_BODY()
private:
	int _maxHp;
	int _maxMp;
public:
	int _hp;
	int _mp;
	int _attack;
	int _defence;
	int _speed;
	FEquipment _equipment;
	bool _fightState;

	UCharacterConfig();
	void ReadConfigData();
	void WriteConfigData();
};
