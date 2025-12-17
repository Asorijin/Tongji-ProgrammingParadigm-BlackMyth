// Fill out your copyright notice in the Description page of Project Settings.

#include "black_moneyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CharacterConfig.h"

void Ublack_moneyGameInstance::Init() {
    UGameInstance::Init();

    eventCenter = NewObject<UEventCenter>(this);
    
}
void Ublack_moneyGameInstance::Shutdown() {
    UGameInstance::Shutdown();
}
