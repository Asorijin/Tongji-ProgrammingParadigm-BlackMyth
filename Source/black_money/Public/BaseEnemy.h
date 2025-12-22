// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyConfig.h"

class USphereComponent;

/**
 * ÊÜ»÷×´Ì¬Ã¶¾Ù
 * ±ØĞëÔÚ.generated.hÖ®Ç°¶¨Òå£¬ÒÔ±ãUE·´ÉäÏµÍ³Ê¶±ğ
 */
UENUM(BlueprintType)
enum class EEnemyHitState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),     // Õı³£×´Ì¬
	Hit			UMETA(DisplayName = "Hit"),        // ÊÜ»÷Ó²Ö±×´Ì¬
	Invulnerable UMETA(DisplayName = "Invulnerable") // ÎŞµĞ×´Ì¬£¨ÊÜ»÷ºó¶ÌÔİÎŞµĞ£©
};

/**
 * AI×´Ì¬Ã¶¾Ù
 * ÓÃÓÚ¹ÜÀí¹ÖÎïµÄAIĞĞÎª×´Ì¬
 */
UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),        // ´ı»ú×´Ì¬
	Chase		UMETA(DisplayName = "Chase"),       // ×·»÷×´Ì¬
	Attack		UMETA(DisplayName = "Attack"),      // ¹¥»÷×´Ì¬
	Dodge		UMETA(DisplayName = "Dodge"),       // ÉÁ±Ü×´Ì¬
	Hit			UMETA(DisplayName = "Hit"),         // ÊÜ»÷×´Ì¬£¨ÓëÊÜ»÷Ó²Ö±×´Ì¬Í¬²½£©
	Dead		UMETA(DisplayName = "Dead")          // ËÀÍö×´Ì¬
};

#include "BaseEnemy.generated.h"

/**
 * ¹ÖÎï»ùÀà
 * ËùÓĞ¹ÖÎïµÄ»ùÀà£¬Ìá¹©Í¨ÓÃ¹¦ÄÜ
 * ¼Ì³Ğ×ÔACharacterÒÔÖ§³ÖÒÆ¶¯ºÍ¶¯»­
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

	// ¹ÖÎïÅäÖÃ¶ÔÏó
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Config")
	UEnemyConfig* EnemyConfig;

	// ¹¥»÷·¶Î§¼ì²â×é¼ş
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	USphereComponent* AttackRangeSphere;

	// ¼ì²â·¶Î§×é¼ş£¨ÓÃÓÚ·¢ÏÖÍæ¼Ò£©
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* DetectionSphere;

public:
	// »ñÈ¡¹ÖÎïÅäÖÃ
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	UEnemyConfig* GetEnemyConfig() const { return EnemyConfig; }

	// ÊÜ»÷´¦Àí£¨Í¨¹ıÊÂ¼şÖĞĞÄµ÷ÓÃ£©
	// ×¢Òâ£ºÖØĞ´»ùÀàAPawnµÄageº¯Êı£¬µ«Ê¹ÓÃ²»Í¬µÄ²ÎÊıÀàĞÍ
	// ÎªÁË±ÜÃâÓë»ùÀàº¯Êı³åÍ»£¬ÎÒÃÇÊ¹ÓÃReceiveDamage×÷ÎªÖ÷Òª½Ó¿Ú
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ReceiveDamage(int32 DamageAmount, AActor* DamageCauser = nullptr);

	// ÖØĞ´»ùÀàµÄTakeDamageº¯Êı£¬ÄÚ²¿µ÷ÓÃReceiveDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ËÀÍö´¦Àí
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	// ¼ì²éÊÇ·ñËÀÍö
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsDead() const;

	// »ñÈ¡µ±Ç°ÊÜ»÷×´Ì¬
	UFUNCTION(BlueprintCallable, Category = "Combat")
	EEnemyHitState GetHitState() const { return CurrentHitState; }

	// ¼ì²éÊÇ·ñ´¦ÓÚÊÜ»÷Ó²Ö±×´Ì¬
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInHitStun() const { return CurrentHitState == EEnemyHitState::Hit; }

	// ¼ì²éÊÇ·ñÎŞµĞ
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInvulnerable() const { return CurrentHitState == EEnemyHitState::Invulnerable; }
	// æ˜¯å¦æ­£åœ¨æ”»å‡»ï¼ˆç»™åŠ¨ç”»è“å›¾ / AnimInstance è¯»å–ï¼‰
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAttacking() const { return bIsAttacking; }

	// ========== AIÏµÍ³Ïà¹Ø·½·¨ ==========
	
	// »ñÈ¡µ±Ç°AI×´Ì¬
	UFUNCTION(BlueprintCallable, Category = "AI")
	EEnemyAIState GetAIState() const { return CurrentAIState; }

	// ÉèÖÃAI×´Ì¬£¨ÄÚ²¿Ê¹ÓÃ£¬×ÓÀà¿ÉÖØĞ´£©
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void SetAIState(EEnemyAIState NewState);

	// ¼ì²éÊÇ·ñÔÚ¹¥»÷·¶Î§ÄÚ
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsPlayerInAttackRange() const;

	// ¼ì²éÊÇ·ñ¼ì²âµ½Íæ¼Ò
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsPlayerDetected() const;

	// »ñÈ¡Íæ¼Ò½ÇÉ«ÒıÓÃ
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ACharacter* GetPlayerCharacter() const;

	// ========== ¹¥»÷ÏµÍ³Ïà¹Ø·½·¨ ==========
	
	/**
	 * Ö´ĞĞ¹¥»÷ÅĞ¶¨£¨ÓÉ¶¯»­Í¨Öªµ÷ÓÃ£©
	 * ²éÕÒ¹¥»÷·¶Î§ÄÚµÄÄ¿±ê²¢Ôì³ÉÉËº¦
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PerformAttack();

	/**
	 * ¿ªÊ¼¹¥»÷£¨²¥·Å¹¥»÷¶¯»­£©
	 * @return ÊÇ·ñ³É¹¦¿ªÊ¼¹¥»÷£¨Èç¹ûÕıÔÚÀäÈ´ÖĞÔò·µ»Øfalse£©
	 */
	virtual bool StartAttack();

	/**
	 * ¼ì²éÊÇ·ñ¿ÉÒÔ¹¥»÷£¨ÀäÈ´Ê±¼äÊÇ·ñ½áÊø£©
	 */
	bool CanAttack() const;

protected:
	// ÊÇ·ñÒÑËÀÍö
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsDead = false;

	// µ±Ç°ÊÜ»÷×´Ì¬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	EEnemyHitState CurrentHitState = EEnemyHitState::Normal;

	// ÊÜ»÷Ó²Ö±Ê±¼ä£¨Ãë£©
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HitStunDuration = 0.3f;

	// ÊÜ»÷ºóÎŞµĞÊ±¼ä£¨Ãë£©
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InvulnerableDuration = 0.5f;

	// ÊÜ»÷Ó²Ö±¼ÆÊ±Æ÷
	FTimerHandle HitStunTimerHandle;

	// ÎŞµĞ×´Ì¬¼ÆÊ±Æ÷
	FTimerHandle InvulnerableTimerHandle;

	// ========== AIÏµÍ³Ïà¹Ø³ÉÔ± ==========
	
	// µ±Ç°AI×´Ì¬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EEnemyAIState CurrentAIState = EEnemyAIState::Idle;

	// Íæ¼Ò½ÇÉ«ÒıÓÃ£¨»º´æ£¬±ÜÃâÃ¿Ö¡²éÕÒ£©
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class ACharacter* PlayerCharacter = nullptr;

	// AI¸üĞÂ¼ä¸ô£¨Ãë£¬±ÜÃâÃ¿Ö¡¶¼¸üĞÂ£¬ÓÅ»¯ĞÔÄÜ£©
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AIUpdateInterval = 0.1f;

	// AI¸üĞÂ¼ÆÊ±Æ÷
	float AIUpdateTimer = 0.0f;

	// ³õÊ¼»¯Õ¼Î»·ûÄ£ĞÍ£¨Mannequin£©
	void InitializePlaceholderMesh();

	// ½øÈëÊÜ»÷Ó²Ö±×´Ì¬
	void EnterHitStun();

	// ½áÊøÊÜ»÷Ó²Ö±×´Ì¬
	void EndHitStun();

	// ½øÈëÎŞµĞ×´Ì¬
	void EnterInvulnerable();

	// ½áÊøÎŞµĞ×´Ì¬
	void EndInvulnerable();

	// »ñÈ¡ÊÂ¼şÖĞĞÄ£¨ÓÃÓÚÍ¨ÖªÉúÃüÖµ±ä»¯µÈ£©
	class UEventCenter* GetEventCenter() const;


	/**
	 * »ñÈ¡¹¥»÷·¶Î§ÄÚµÄÊÜ»÷Ä¿±ê£¨±ã½İ·½·¨£¬Ä¬ÈÏ²éÕÒPlayer±êÇ©£©
	 * ²Î¿¼½ÇÉ«ÀàµÄGetNearbyObjectsWithTag·½·¨ÊµÏÖ
	 * @param AttackRange ¹¥»÷·¶Î§°ë¾¶£¨Èç¹û<=0£¬ÔòÊ¹ÓÃAttackRangeSphereµÄ°ë¾¶£©
	 * @return ¹¥»÷·¶Î§ÄÚµÄÊÜ»÷¶ÔÏóÁĞ±í£¨Ä¬ÈÏ²éÕÒ"Player"±êÇ©£©
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	TArray<AActor*> GetAttackTargetsInRange(float AttackRange = 0.0f) const;

	/**
	 * »ñÈ¡¹¥»÷·¶Î§ÄÚµÄÊÜ»÷Ä¿±ê£¨ÍêÕû°æ±¾£¬C++ÄÚ²¿Ê¹ÓÃ£©
	 * @param AttackRange ¹¥»÷·¶Î§°ë¾¶£¨Èç¹û<=0£¬ÔòÊ¹ÓÃAttackRangeSphereµÄ°ë¾¶£©
	 * @param TargetTags Ä¿±ê±êÇ©ÁĞ±í
	 * @return ¹¥»÷·¶Î§ÄÚµÄÊÜ»÷¶ÔÏóÁĞ±í
	 */
	TArray<AActor*> GetAttackTargetsInRangeWithTags(float AttackRange, const TArray<FName>& TargetTags) const;

	// ========== AIÏµÍ³ÄÚ²¿·½·¨ ==========
	
	// ¸üĞÂAI×´Ì¬£¨ÔÚTickÖĞµ÷ÓÃ£©
	virtual void UpdateAI(float DeltaTime);

	// ¼ì²âÍæ¼Ò£¨ÔÚDetectionSphere·¶Î§ÄÚ£©
	// ×¢Òâ£º´Ë·½·¨»á¸üĞÂPlayerCharacterÒıÓÃ£¬ËùÒÔ²»ÊÇconst
	virtual bool DetectPlayer();

	// ¼ÆËãµ½Íæ¼ÒµÄ¾àÀë
	float GetDistanceToPlayer() const;

	// Ö´ĞĞ×·»÷ÒÆ¶¯
	virtual void ChasePlayer(float DeltaTime);

	// Í£Ö¹ÒÆ¶¯
	void StopMovement();

	// ×´Ì¬ÇĞ»»Âß¼­£¨¸ù¾İµ±Ç°Çé¿ö¾ö¶¨ÏÂÒ»¸ö×´Ì¬£©
	virtual EEnemyAIState DetermineNextState() const;

	/**
	 * ¹¥»÷¶¯»­²¥·ÅÍê³É»Øµ÷
	 */
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	// ========== ¹¥»÷ÏµÍ³Ïà¹Ø³ÉÔ± ==========
	
	// ¹¥»÷¶¯»­ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* AttackMontage;

	// ÊÇ·ñÕıÔÚ¹¥»÷£¨ÓÃÓÚ·ÀÖ¹ÖØ¸´ÅĞ¶¨£©
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking = false;

	// ¹¥»÷ÀäÈ´¼ÆÊ±Æ÷
	FTimerHandle AttackCooldownTimer;

	// µ±Ç°¹¥»÷ÀäÈ´Ê£ÓàÊ±¼ä£¨ÓÃÓÚµ÷ÊÔºÍUIÏÔÊ¾£©
	float AttackCooldownRemaining = 0.0f;

	// ±¾´Î¹¥»÷ÖĞÒÑÃüÖĞµÄÄ¿±ê£¨·ÀÖ¹Í¬Ò»¹¥»÷¶¯»­ÖĞÖØ¸´ÅĞ¶¨£©
	UPROPERTY()
	TArray<AActor*> AlreadyHitTargetsInThisAttack;

	// ========== ÊÜ»÷ºÍËÀÍö¶¯»­ÃÉÌ«Ææ ==========
	
	// ÊÜ»÷¶¯»­ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* HitMontage;

	// ËÀÍö¶¯»­ÃÉÌ«Ææ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* DeathMontage;
};

