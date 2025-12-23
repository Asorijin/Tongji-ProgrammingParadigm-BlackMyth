// Copyright Epic Games, Inc. All Rights Reserved.

#include "black_moneyGameMode.h"
#include "black_moneyCharacter.h"
#include "black_moneyGameInstance.h"
#include "EventCenter.h"
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

void Ablack_moneyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);
	if (Ublack_moneyGameInstance* GI = Cast<Ublack_moneyGameInstance>(GetGameInstance())) {
		UEventCenter* eventCenter = GI->GetEventCenter();
		eventCenter->GenerateActors();
	}
}
