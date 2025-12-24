// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EventCenter.h"
#include "CharacterConfig.h"
#include "black_moneyGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class BLACK_MONEY_API Ublack_moneyGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	/*改成public*/
	//virtual void Init() override;
	//virtual void Shutdown() override;

	UPROPERTY()
	UEventCenter* eventCenter;
public:

	
	/*
	在使用时，在需要使用的cpp文件中include EventCenter头文件，Cast转为UEventCenter再调用事件中心的方法
	绝对不要随意在头文件中引用EventCenter头文件，出现循环引用改起来会很麻烦
	强烈不建议调用该方法，建议通过引擎获取GameInstance调用对应方法
	*/
	UEventCenter* GetEventCenter() const { return eventCenter; }
<<<<<<< HEAD


	UPROPERTY(VisibleAnywhere, Category = "Launch")
	bool bIsFirstLaunch;


	virtual void Init() override;
    virtual void Shutdown() override;
=======
>>>>>>> bd214ed4390b14a17c404e6808c277451f38b824
};
