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
void Ublack_moneyGameInstance::SwitchToLevel(const FString& LevelName, FVector SpawnLocation) {
    //目前为止仅设定了关卡和出生点，还需要在角色BeginPlay中调用移动，才能将角色移动过去
    pawnLastLocation = SpawnLocation;

    if (UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevel(World, FName(*LevelName));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SwitchToLevel: No valid world!"));
    }
 }
