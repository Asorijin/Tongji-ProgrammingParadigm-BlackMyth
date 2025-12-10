// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tools.generated.h"

UCLASS()
class BLACK_MONEY_API ATools : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATools();
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//0:hp  1:mp  2:attack  3:defence  4:speed
	TArray<int> attributeVector = {0,0,0,0,0};

	UPROPERTY()
	int toolNumber = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseTools();

	virtual int GetToolNumber();
};

