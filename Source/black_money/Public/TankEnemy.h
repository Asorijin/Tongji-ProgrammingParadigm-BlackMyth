// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "TankEnemy.generated.h"

/**
 * 坦克怪物类
 * 高防御高血量型怪物：高血量、高防御、低攻击、低机动性
 */
UCLASS()
class BLACK_MONEY_API ATankEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ATankEnemy(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

