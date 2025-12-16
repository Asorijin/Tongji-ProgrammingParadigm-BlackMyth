// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyConfig.h"

class USphereComponent;

/**
 * 受击状态枚举
 * 必须在.generated.h之前定义，以便UE反射系统识别
 */
UENUM(BlueprintType)
enum class EEnemyHitState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),     // 正常状态
	Hit			UMETA(DisplayName = "Hit"),        // 受击硬直状态
	Invulnerable UMETA(DisplayName = "Invulnerable") // 无敌状态（受击后短暂无敌）
};

#include "BaseEnemy.generated.h"

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
	// 注意：重写基类APawn的TakeDamage函数，但使用不同的参数类型
	// 为了避免与基类函数冲突，我们使用ReceiveDamage作为主要接口
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ReceiveDamage(int32 DamageAmount, AActor* DamageCauser = nullptr);

	// 重写基类的TakeDamage函数，内部调用ReceiveDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 死亡处理
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	// 检查是否死亡
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsDead() const;

	// 获取当前受击状态
	UFUNCTION(BlueprintCallable, Category = "Combat")
	EEnemyHitState GetHitState() const { return CurrentHitState; }

	// 检查是否处于受击硬直状态
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInHitStun() const { return CurrentHitState == EEnemyHitState::Hit; }

	// 检查是否无敌
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInvulnerable() const { return CurrentHitState == EEnemyHitState::Invulnerable; }

protected:
	// 是否已死亡
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsDead = false;

	// 当前受击状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	EEnemyHitState CurrentHitState = EEnemyHitState::Normal;

	// 受击硬直时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HitStunDuration = 0.3f;

	// 受击后无敌时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InvulnerableDuration = 0.5f;

	// 受击硬直计时器
	FTimerHandle HitStunTimerHandle;

	// 无敌状态计时器
	FTimerHandle InvulnerableTimerHandle;

	// 初始化占位符模型（Mannequin）
	void InitializePlaceholderMesh();

	// 进入受击硬直状态
	void EnterHitStun();

	// 结束受击硬直状态
	void EndHitStun();

	// 进入无敌状态
	void EnterInvulnerable();

	// 结束无敌状态
	void EndInvulnerable();

	// 获取事件中心（用于通知生命值变化等）
	class UEventCenter* GetEventCenter() const;

	/**
	 * 获取攻击范围内的受击目标（便捷方法，默认查找Player标签）
	 * 参考角色类的GetNearbyObjectsWithTag方法实现
	 * @param AttackRange 攻击范围半径（如果<=0，则使用AttackRangeSphere的半径）
	 * @return 攻击范围内的受击对象列表（默认查找"Player"标签）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	TArray<AActor*> GetAttackTargetsInRange(float AttackRange = 0.0f) const;

	/**
	 * 获取攻击范围内的受击目标（完整版本，C++内部使用）
	 * @param AttackRange 攻击范围半径（如果<=0，则使用AttackRangeSphere的半径）
	 * @param TargetTags 目标标签列表
	 * @return 攻击范围内的受击对象列表
	 */
	TArray<AActor*> GetAttackTargetsInRangeWithTags(float AttackRange, const TArray<FName>& TargetTags) const;
};

