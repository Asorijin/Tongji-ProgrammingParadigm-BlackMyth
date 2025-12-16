// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Tools.generated.h"

UCLASS()
class BLACK_MONEY_API ATools : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATools();
private:
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseTools();

};

