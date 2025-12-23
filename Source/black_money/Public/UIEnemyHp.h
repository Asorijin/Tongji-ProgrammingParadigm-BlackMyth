// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIEnemyHp.generated.h"

/**
 * 
 */
UCLASS()
class BLACK_MONEY_API UUIEnemyHp : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 _hp;

};
