// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyConfig.h"
#include "BaseEnemy.generated.h"

class USphereComponent;

/**
 * 怪物基类
 * 所有怪物的基类，提供通用功能
 * 继承自ACharacter以支持移动和动画
 */
UCLASS()
class BLACK_MONEY_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 怪物配置对象
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Config")
	UEnemyConfig* EnemyConfig;

	// 攻击范围检测组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	USphereComponent* AttackRangeSphere;

	// 检测范围组件（用于发现玩家）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* DetectionSphere;

public:
	// 获取怪物配置
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	UEnemyConfig* GetEnemyConfig() const { return EnemyConfig; }

	// 受击处理（通过事件中心调用）
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void TakeDamage(int32 DamageAmount);

	// 死亡处理
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	// 检查是否死亡
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsDead() const;

protected:
	// 是否已死亡
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsDead = false;

	// 初始化占位符模型（Mannequin）
	void InitializePlaceholderMesh();
};

