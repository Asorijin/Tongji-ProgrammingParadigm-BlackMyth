// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TimerManager.h"
#include <Components/SphereComponent.h>
#include <Components/AudioComponent.h>
#include "CharacterConfig.h"
#include"EventCenter.h"
#include "black_moneyCharacter.generated.h"
class UBoxComponent;

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class Ablack_moneyCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	// 添加 DodgeAction 指针
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	//添加攻击动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	//添加技能1-棒震大地（Q键）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillEarthQuakeAction;
	//绑定拾取道具动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickUpAction;


	// 暂停功能的输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseAction;
	// 使用血量道具输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UseHealthItemAction;
	// 使用蓝量道具输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UseManaItemAction;

public:
	Ablack_moneyCharacter();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	/** 闪避功能（按 Ctrl 触发） */
	void Dodge();

	/** 闪避条件检查 */
	bool CanDodge() const;


	//攻击功能
	void Attack();
	//处理死亡
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void HandleDeath();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* DetectionSphere;

	//控制音乐\循环音效
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* BackGroundMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* nextBackGroundMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterConfig* characterConfig;

	// 蓝量 / 技能数值
protected:

	// 每秒回蓝
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Mana")
	float ManaRegenPerSecond = 2.0f;
	// 蓝量回满所需时间累积器
	float MpRegenAccumulator = 0.0f;


	// 技能蓝耗
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Config")
	float EarthQuakeManaCost = 30.0f;

	// 技能冷却时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Config")
	float EarthQuakeCooldown = 10.0f;

	// 距离上次释放该技能的时间（秒），用负值/大于冷却来表示可用
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|State")
	float EarthQuakeCooldownRemaining = 0.0f;

	// 技能蒙太奇（震地动画）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EarthQuakeMontage = nullptr;
public:
		// 技能时长只读接口（给 UI / AnimBP 用）
		UFUNCTION(BlueprintPure, Category = "Skill|State")
		float GetEarthQuakeCooldownRemaining() const { return EarthQuakeCooldownRemaining; }

private:
	// 内部检查是否能放技能
	bool CanCastEarthQuake() const;

	// 处理暂停输入的函数
	void HandlePauseInput();
public:
	// 真正结算震地伤害（可在 AnimNotify 里调用）
	UFUNCTION(BlueprintCallable, Category = "Skill")

	void DoEarthQuakeDamage();

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//判断是否处于闪避中
	FORCEINLINE bool IsDodging() const { return bIsDodging; };
	//判断是否处于攻击状态
	FORCEINLINE bool IsAttacking() const { return bIsAttacking; };
	//判断是否处于受击中
	FORCEINLINE bool IsTakingDamage() const { return bIsTakingDamage; }
	//获取附近可交互物体
	TSet<AActor*> nearbyInteraction;
	// 技能：棒震大地
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void CastEarthQuake();


	void BeginPlay() override;

	void Tick(float deltaTime) override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	const UCharacterConfig* ShareCharacterConfig();
private:
	/** 闪避强度（水平冲量） */
	UPROPERTY(EditAnywhere, Category = "Dodge")
	float DodgeStrength = 1200.0f;


	/** 是否处于闪避中 */
	UPROPERTY(VisibleAnywhere, Category = "Dodge")
	bool bIsDodging = false;

	/** 闪避期间免伤标记 */
	UPROPERTY(VisibleAnywhere, Category = "Dodge")
	bool bInvulnerableDuringDodge = true;

	
	//是否处于攻击状态
	bool bIsAttacking = false;
	// 是否处于受击中
	UPROPERTY(VisibleAnywhere, Category = "Hit")
	bool bIsTakingDamage = false;
	// 是否已死亡
	UPROPERTY(VisibleAnywhere,  Category = "State")
	bool bIsDead = false;
	// 是否处于释放技能中（例如棒震大地）
	UPROPERTY(VisibleAnywhere, Category = "Skill|State")
	bool bIsCastingSkill = false;

protected:
	// 武器碰撞回调
	UFUNCTION()
	void OnWeaponHitBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	FName GetHitSectionNameForCauser(const AActor* Victim, const AActor* DamageCauser);
	// ----- 武器判定组件/数据 -----
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* WeaponHitBox;

	// 本次攻击已命中的目标，避免重复命中
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	// 事件中心指针
	UPROPERTY()
	UEventCenter* EventCenter = nullptr;
	//攻击次数
	int ComboIndex = 0;
	// 是否已经在当前段的“可连击窗口”里
	bool bCanQueueNextCombo = false;

	// 玩家是否在本段期间按过攻击键（排队标志）
	bool bQueuedNextCombo = false;
	// 攻击连击蒙太奇（蓝图中指定）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;
	/** 闪避翻滚蒙太奇（在角色蓝图里指定） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage = nullptr;
	/** 受击蒙太奇（四向受击） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage = nullptr;
	// 死亡蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;
	float DodgePlayRate = 1.0f;

	//改变音乐
	void ChangeMusic(FName musicName);

	/*需要在CharacterMenu中调用，改为public*/
	//const UCharacterConfig* ShareCharacterConfig();

	//按F触发与物体互动事件，需要绑定按键
	void TriggerNearByInteractions();
	//使用血量道具
	void UseToolHp();
	//使用法力道具
	void UseToolMp();
	public:
		// 重写 UE 内置的 TakeDamage
		virtual float TakeDamage(
			float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			class AActor* DamageCauser
		) override;

	public:
		UFUNCTION(BlueprintCallable, Category = "Combat")
		void EnableComboWindow();

		UFUNCTION(BlueprintCallable, Category = "Combat")
		void OnAttackSectionEnded();
		//开启伤害判定
		UFUNCTION(BlueprintCallable, Category = "Combat")
		void StartAttackHit();
		//结束伤害判定
		UFUNCTION(BlueprintCallable, Category = "Combat")
		void EndAttackHit();
		/** 闪避结束回调 */
		void EndDodge();
		// 受击结束回调
		void EndHit() {
			bIsTakingDamage = false;
			return;
		}
		// 给 AnimInstance 用的只读接口
		UFUNCTION(BlueprintPure, Category = "State")
		bool IsDead() const { return bIsDead; }

		UFUNCTION(BlueprintPure, Category = "Skill|State")
		bool IsCastingSkill() const { return bIsCastingSkill; }
		// 技能释放结束（在动画通知中调用）
		UFUNCTION(BlueprintCallable, Category = "Skill|State")
		void EndCastSkill() { bIsCastingSkill = false; }

};

