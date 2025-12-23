// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "ArcherEnemy.generated.h"

/**
 * 弓箭手怪物类
 * 远程输出型怪物：低血量、低防御、高攻击、高机动性
 */
UCLASS()
class BLACK_MONEY_API AArcherEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AArcherEnemy(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

