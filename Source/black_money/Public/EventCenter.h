// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EventCenter.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API UEventCenter : public UObject
{
	GENERATED_BODY()
private:
	void GenerateMonster();
	void ChangeBGM();
	void PlayMusic(std::string musicName);

public:
	void MakeDamage(TSubclassOf<AActor> makeDamager, TSubclassOf<AActor> takeDamager, float damageNumber);
	//需要先实现道具类数据结构体
	void UseTools();
	void GetTools(size_t toolNumber);
	//需要先实现装备结构体
	void ChangeEquipment();
};
