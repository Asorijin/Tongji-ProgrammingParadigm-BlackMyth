// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools.h"
#include "ToolHp.generated.h"

UCLASS()
class BLACK_MONEY_API AToolHp : public ATools
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToolHp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseTools() override;

	//向背包面板提供道具数量
	int GetToolNumber() override;
};
