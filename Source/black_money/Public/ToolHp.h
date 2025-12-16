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
	
	// 交互触发器（球形碰撞）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* InteractionTrigger;

	// 浮动 UI 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractionWidget;

	APlayerController* CachedPlayerController = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* meshComponent;

	FVector CameraLocation;

	FRotator CameraRotation;

	FVector WidgetLocation;

	FVector LookAtDirection;

	// 重叠开始时调用
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 重叠结束时调用
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseTools() override;

};
