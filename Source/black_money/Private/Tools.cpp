// Fill out your copyright notice in the Description page of Project Settings.

#include "Tools.h"
#include "black_moneyGameInstance.h"
// Sets default values
ATools::ATools()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATools::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATools::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATools::UseTools() {
	if (toolNumber > 0) {
		Cast<Ublack_moneyGameInstance>(GetGameInstance())->eventCenter->UseTools(&attributeVector);
		toolNumber--;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Tool: toolNumber wrong!"));
	}
}

int ATools::GetToolNumber() {
	return toolNumber;
}
