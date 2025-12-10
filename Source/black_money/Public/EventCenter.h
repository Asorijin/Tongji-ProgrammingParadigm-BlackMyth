// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools.h"
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
	UPROPERTY()
	FVector pawnLastLocation;

	UEventCenter();
	void SwitchToLevel(const FString& LevelName, FVector SpawnLocation);
	void MakeDamage(TSubclassOf<AActor> makeDamager, TSubclassOf<AActor> takeDamager, float damageNumber);
	void UseTools(TArray<int>* attributeVector);
	void GetTools(TSubclassOf<ATools>* tool,int toolNumber);
	void ChangeEquipment();
};
