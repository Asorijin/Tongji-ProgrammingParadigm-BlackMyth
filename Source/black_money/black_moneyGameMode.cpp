// Copyright Epic Games, Inc. All Rights Reserved.

#include "black_moneyGameMode.h"
#include "black_moneyCharacter.h"
#include "UObject/ConstructorHelpers.h"

Ablack_moneyGameMode::Ablack_moneyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
