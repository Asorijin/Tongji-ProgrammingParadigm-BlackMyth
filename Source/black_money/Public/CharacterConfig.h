// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ToolHp.h"
#include "CharacterConfig.generated.h"
/**
 * 初步计划是Character类在初始化时使用该类（通过GameInstance间接，不可以直接Character类include该类）读取配置文件，初始化数值
 * 游戏过程中角色的数值存储在该类中
 */
UCLASS()
class BLACK_MONEY_API UCharacterConfig : public UObject
{
	GENERATED_BODY()
private:
	int32 _maxHp = 100;//最大生命值
	int32 _maxMp = 100;//最大魔法值
	FString filePath = FPaths::ProjectSavedDir() / TEXT("Config/GameConfigs/CharacterConfig.txt");
public:
	int32 _hp = 100;
	int32 _mp = 100;
	int32 _attack = 30;
	int32 _defence = 30;
	bool _fightState = false;

	UCharacterConfig();
	void Initialize();

	//写文件操作放在和土地庙的时候，在显示土地庙面板前写入，直到写入完成再打开面板
	void WriteConfigData();
	int32 GetMaxHp() const { return _maxHp; }
	int32 GetMaxMp() const { return _maxMp; }
	
};
