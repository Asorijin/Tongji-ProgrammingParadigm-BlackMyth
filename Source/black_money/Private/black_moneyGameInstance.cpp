// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "black_moneyGameInstance.h"

void Ublack_moneyGameInstance::Init() {
    Super::Init();

    eventCenter = NewObject<UEventCenter>(this);
    if (eventCenter)
    {
        eventCenter->AddToRoot();
    }
}
