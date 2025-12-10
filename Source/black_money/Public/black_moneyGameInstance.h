// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EventCenter.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "black_moneyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API Ublack_moneyGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	virtual void Init() override;

	void SwitchToLevel(const FString& LevelName, FVector SpawnLocation);

	UEventCenter* eventCenter;

	UEventCenter* GetEventCenter() const { return eventCenter; }

	FVector pawnLastLocation;
};
