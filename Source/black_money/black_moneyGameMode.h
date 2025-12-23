// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "black_moneyGameMode.generated.h"

UCLASS(minimalapi)
class Ablack_moneyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	Ablack_moneyGameMode();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};



