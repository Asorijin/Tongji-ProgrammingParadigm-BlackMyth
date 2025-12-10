// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolHp.h"

// Sets default values
AToolHp::AToolHp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	attributeVector[0] = 50;
}

// Called when the game starts or when spawned
void AToolHp::BeginPlay()
{
	ATools::BeginPlay();
}

// Called every frame
void AToolHp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AToolHp::UseTools() {
	ATools::UseTools();
}

int AToolHp::GetToolNumber() {
	return ATools::GetToolNumber();
}

