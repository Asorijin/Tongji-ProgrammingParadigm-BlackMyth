// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EventCenter.h"
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
public:

	UPROPERTY()
	UEventCenter* eventCenter;

	UEventCenter* GetEventCenter() const { return eventCenter; }

};
