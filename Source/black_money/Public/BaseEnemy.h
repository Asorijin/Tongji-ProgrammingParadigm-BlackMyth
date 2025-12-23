// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
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

/**
 * AI状态枚举
 * 用于管理怪物的AI行为状态
 */
UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),        // 待机状态
	Chase		UMETA(DisplayName = "Chase"),       // 追击状态
	Attack		UMETA(DisplayName = "Attack"),      // 攻击状态
	Dodge		UMETA(DisplayName = "Dodge"),       // 闪避状态
	Hit			UMETA(DisplayName = "Hit"),         // 受击状态（与受击硬直状态同步）
	Dead		UMETA(DisplayName = "Dead")          // 死亡状态
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

	// 怪物配置对象（可在类默认值中编辑）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Enemy Config")
	UEnemyConfig* EnemyConfig;

	// 攻击范围检测组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	USphereComponent* AttackRangeSphere;

	// 检测范围组件（用于发现玩家）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* DetectionSphere;

	// 交互触发器（球形碰撞）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* InteractionTrigger;

	// 浮动 UI 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractionWidget;

	APlayerController* CachedPlayerController = nullptr;

public:
	// 获取怪物配置
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	UEnemyConfig* GetEnemyConfig() const { return EnemyConfig; }

	// 受击处理（通过事件中心调用）
	// 注意：重写基类APawn的age函数，但使用不同的参数类型
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
	// 鏄?鍚︽?ｅ湪鏀诲嚮锛堢粰鍔ㄧ敾钃濆浘 / AnimInstance 璇诲彇锛?
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAttacking() const { return bIsAttacking; }

	// 是否正在闪避（给动画蓝图 / AnimInstance 读取）
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	bool IsDodging() const { return bIsDodging; }

	// ========== AI系统相关方法 ==========
	
	// 获取当前AI状态
	UFUNCTION(BlueprintCallable, Category = "AI")
	EEnemyAIState GetAIState() const { return CurrentAIState; }

	// 设置AI状态（内部使用，子类可重写）
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void SetAIState(EEnemyAIState NewState);

	// 检查是否在攻击范围内
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsPlayerInAttackRange() const;

	// 检查是否检测到玩家
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsPlayerDetected() const;

	// 获取玩家角色引用
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ACharacter* GetPlayerCharacter() const;

	// ========== 攻击系统相关方法 ==========
	
	/**
	 * 执行攻击判定（由动画通知调用）
	 * 查找攻击范围内的目标并造成伤害
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PerformAttack();

	/**
	 * 开始攻击（播放攻击动画）
	 * @return 是否成功开始攻击（如果正在冷却中则返回false）
	 */
	virtual bool StartAttack();

	/**
	 * 检查是否可以攻击（冷却时间是否结束）
	 */
	bool CanAttack() const;

	// ========== 闪避系统相关方法 ==========
	
	/**
	 * 判断是否可以闪避
	 */
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	bool CanDodge() const;

	/**
	 * 判断是否应该闪避（基于距离和概率）
	 */
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	bool ShouldDodge() const;

	/**
	 * 执行闪避（播放闪避动画并执行闪避移动）
	 */
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void PerformDodge();

	/**
	 * 检测玩家是否正在攻击
	 */
	bool IsPlayerAttacking() const;

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

	// ========== AI系统相关成员 ==========
	
	// 当前AI状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EEnemyAIState CurrentAIState = EEnemyAIState::Idle;

	// 玩家角色引用（缓存，避免每帧查找）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class ACharacter* PlayerCharacter = nullptr;

	// AI更新间隔（秒，避免每帧都更新，优化性能）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AIUpdateInterval = 0.1f;

	// AI更新计时器
	float AIUpdateTimer = 0.0f;

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

	// ========== AI系统内部方法 ==========
	
	// 更新AI状态（在Tick中调用）
	virtual void UpdateAI(float DeltaTime);

	// 检测玩家（在DetectionSphere范围内）
	// 注意：此方法会更新PlayerCharacter引用，所以不是const
	virtual bool DetectPlayer();

	// 计算到玩家的距离
	float GetDistanceToPlayer() const;

	// 执行追击移动
	virtual void ChasePlayer(float DeltaTime);

	// 停止移动
	void StopMovement();

	// 状态切换逻辑（根据当前情况决定下一个状态）
	virtual EEnemyAIState DetermineNextState() const;

	/**
	 * 攻击动画播放完成回调
	 */
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 重叠开始时调用
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 重叠结束时调用
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// ========== 攻击系统相关成员 ==========
	
	// 攻击动画蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* AttackMontage;

	// 是否正在攻击（用于防止重复判定）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking = false;

	// 攻击冷却计时器
	FTimerHandle AttackCooldownTimer;

	// 当前攻击冷却剩余时间（用于调试和UI显示）
	float AttackCooldownRemaining = 0.0f;

	// 本次攻击中已命中的目标（防止同一攻击动画中重复判定）
	UPROPERTY()
	TArray<AActor*> AlreadyHitTargetsInThisAttack;

	// ========== 受击和死亡动画蒙太奇 ==========
	
	// 受击动画蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* HitMontage;

	// 死亡动画蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* DeathMontage;

	// ========== 闪避系统相关成员 ==========
	
	// 闪避动画蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	class UAnimMontage* DodgeMontage;

	// 是否正在闪避
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dodge")
	bool bIsDodging = false;

	// 闪避期间是否无敌
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dodge")
	bool bInvulnerableDuringDodge = false;

	// 闪避概率（0.0-1.0）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeProbability = 0.5f;

	// 检测玩家攻击的范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeRange = 300.0f;

	// 闪避强度（移动速度）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeStrength = 800.0f;

	// 闪避持续时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeDuration = 0.3f;

	// 闪避冷却时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeCooldown = 2.0f;

	// 闪避计时器
	FTimerHandle DodgeTimerHandle;

	// 闪避冷却计时器
	FTimerHandle DodgeCooldownTimerHandle;

	// 闪避蒙太奇结束回调
	UFUNCTION()
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 结束闪避（备用方法）
	void EndDodge();

	// 闪避冷却结束回调
	void OnDodgeCooldownEnd();
};

