// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "BossEnemy.generated.h"

class UParticleSystemComponent;
class UParticleSystem;

/**
 * Boss阶段枚举
 */
UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1		UMETA(DisplayName = "Phase1"),      // 第一阶段
	Phase2		UMETA(DisplayName = "Phase2"),      // 第二阶段
	PhaseTransition UMETA(DisplayName = "PhaseTransition") // 阶段转换中（播放咆哮动画）
};

/**
 * Boss敌人类
 * 继承BaseEnemy，实现多阶段战斗、怒气值系统和技能系统
 */
UCLASS()
class BLACK_MONEY_API ABossEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ABossEnemy(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ========== 阶段系统 ==========
	
	// 当前阶段
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Phase")
	EBossPhase CurrentPhase = EBossPhase::Phase1;

	// 第一阶段攻击蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Combat")
	UAnimMontage* Phase1AttackMontage = nullptr;

	// 第二阶段攻击蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Combat")
	UAnimMontage* Phase2AttackMontage = nullptr;

	// 咆哮动画（阶段转换用）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Combat")
	UAnimMontage* RoarMontage = nullptr;

	// 第二阶段粒子效果资源（用于配置）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Combat")
	UParticleSystem* Phase2ParticleEffect = nullptr;

	// 第二阶段粒子效果组件（持续播放）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Combat")
	class UParticleSystemComponent* Phase2ParticleComponent = nullptr;

	// 第二阶段攻击伤害倍率（相对于基础攻击力）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Combat", meta = (ClampMin = "1.0"))
	float Phase2AttackMultiplier = 1.5f;

	// 第二阶段生命回复速度（每秒回复的生命值）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Combat", meta = (ClampMin = "0.0"))
	float HealthRegenRate = 5.0f;

	// 生命回复定时器句柄
	FTimerHandle HealthRegenTimerHandle;

	// ========== 怒气值系统 ==========
	
	// 当前怒气值
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Rage")
	float CurrentRageValue = 0.0f;

	// 最大怒气值（达到此值可触发技能）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Rage", meta = (ClampMin = "1.0"))
	float MaxRageValue = 100.0f;

	// 每次受到伤害增加的怒气值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Rage", meta = (ClampMin = "0.0"))
	float RagePerDamage = 1.0f;

	// ========== 技能系统 ==========
	
	// 技能前摇蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill")
	UAnimMontage* SkillWindupMontage = nullptr;

	// 技能攻击蒙太奇（跳跃攻击）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill")
	UAnimMontage* SkillAttackMontage = nullptr;

	// 是否正在使用技能
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Skill")
	bool bIsUsingSkill = false;

	// 技能伤害（独立于普通攻击）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill", meta = (ClampMin = "0.0"))
	int32 SkillDamage = 50;

	// 技能攻击范围（跳跃落地后的伤害范围）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill", meta = (ClampMin = "0.0"))
	float SkillAttackRange = 300.0f;

	// 技能冷却时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill", meta = (ClampMin = "0.0"))
	float SkillCooldown = 10.0f;

	// 技能冷却剩余时间
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Skill")
	float SkillCooldownRemaining = 0.0f;

	// 技能定时器句柄
	FTimerHandle SkillCooldownTimerHandle;

public:
	// ========== 重写父类函数 ==========
	
	// 重写受伤害函数，添加怒气值系统
	virtual void ReceiveDamage(int32 DamageAmount, AActor* DamageCauser = nullptr) override;

	// 重写执行攻击函数，根据阶段使用不同的攻击力和伤害
	virtual void PerformAttack() override;

	// 重写开始攻击函数，根据阶段选择不同的攻击动画
	virtual bool StartAttack() override;

	// 重写攻击蒙太奇结束回调（不需要UFUNCTION宏，继承父类的UFUNCTION）
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ========== Boss专用函数 ==========
	
	// 获取是否正在使用技能（用于动画蓝图实际使用）
	UFUNCTION(BlueprintCallable, Category = "Boss Skill")
	bool IsUsingSkill() const { return bIsUsingSkill; }

	// 获取当前阶段
	UFUNCTION(BlueprintCallable, Category = "Boss Phase")
	EBossPhase GetCurrentPhase() const { return CurrentPhase; }

	// 检查并触发阶段转换
	UFUNCTION(BlueprintCallable, Category = "Boss Phase")
	void CheckPhaseTransition();

	// 进入第二阶段
	UFUNCTION(BlueprintCallable, Category = "Boss Phase")
	void EnterPhase2();

	// 播放咆哮动画
	UFUNCTION(BlueprintCallable, Category = "Boss Phase")
	void PlayRoarAnimation();

	// 咆哮动画结束回调
	UFUNCTION()
	void OnRoarMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ========== 怒气值系统函数 ==========
	
	// 获取当前怒气值
	UFUNCTION(BlueprintCallable, Category = "Boss Rage")
	float GetCurrentRageValue() const { return CurrentRageValue; }

	// 获取怒气值百分比
	UFUNCTION(BlueprintCallable, Category = "Boss Rage")
	float GetRagePercentage() const { return MaxRageValue > 0.0f ? CurrentRageValue / MaxRageValue : 0.0f; }

	// 增加怒气值
	UFUNCTION(BlueprintCallable, Category = "Boss Rage")
	void AddRageValue(float Amount);

	// 获取技能前摇蒙太奇（用于动画蓝图判断）
	UFUNCTION(BlueprintCallable, Category = "Boss Skill")
	UAnimMontage* GetSkillWindupMontage() const { return SkillWindupMontage; }

	// 获取技能攻击蒙太奇（用于动画蓝图判断）
	UFUNCTION(BlueprintCallable, Category = "Boss Skill")
	UAnimMontage* GetSkillAttackMontage() const { return SkillAttackMontage; }

	// 检查是否可以释放技能
	UFUNCTION(BlueprintCallable, Category = "Boss Skill")
	bool CanUseSkill() const;

	// 释放技能（跳跃攻击）
	UFUNCTION(BlueprintCallable, Category = "Boss Skill")
	void UseSkill();

	// 技能前摇蒙太奇结束回调
	UFUNCTION()
	void OnSkillWindupMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 技能攻击蒙太奇结束回调
	UFUNCTION()
	void OnSkillAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 执行技能伤害判定（在技能动画特定帧调用）
	UFUNCTION(BlueprintCallable, Category = "Boss Skill")
	void PerformSkillAttack();

	// 技能冷却结束回调
	UFUNCTION()
	void OnSkillCooldownEnd();

	// ========== 生命回复系统 ==========
	
	// 开始生命回复（第二阶段）
	UFUNCTION(BlueprintCallable, Category = "Boss Combat")
	void StartHealthRegen();

	// 停止生命回复
	UFUNCTION(BlueprintCallable, Category = "Boss Combat")
	void StopHealthRegen();

	// 生命回复Tick函数
	UFUNCTION()
	void TickHealthRegen();

protected:
	// AI更新函数，重写以处理阶段和技能逻辑
	virtual void UpdateAI(float DeltaTime) override;

	// 获取当前阶段对应的攻击蒙太奇
	UAnimMontage* GetCurrentPhaseAttackMontage() const;

	// 获取当前阶段对应的攻击伤害
	int32 GetCurrentPhaseAttackDamage() const;
};
