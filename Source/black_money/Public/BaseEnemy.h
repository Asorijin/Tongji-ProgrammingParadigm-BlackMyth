// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "EnemyConfig.h"

class USphereComponent;
/**
 * �ܻ�״̬ö��
 * ������.generated.h֮ǰ���Ա�UE����ϵͳʶ��
 */
UENUM(BlueprintType)
enum class EEnemyHitState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),     // ����״̬
	Hit			UMETA(DisplayName = "Hit"),        // �ܻ�Ӳֱ״̬
	Invulnerable UMETA(DisplayName = "Invulnerable") // �޵�״̬���ܻ�������޵У�
};

/**
 * AI״̬ö��
 * ���ڹ��������AI��Ϊ״̬
 */
UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),        // ����״̬
	Chase		UMETA(DisplayName = "Chase"),       // ׷��״̬
	Attack		UMETA(DisplayName = "Attack"),      // ����״̬
	Dodge		UMETA(DisplayName = "Dodge"),       // ����״̬
	Hit			UMETA(DisplayName = "Hit"),         // �ܻ�״̬�����ܻ�Ӳֱ״̬ͬ����
	Dead		UMETA(DisplayName = "Dead")          // ����״̬
};

#include "BaseEnemy.generated.h"

/**
 * ����������
 * ���е��˵Ļ��࣬�ṩͨ�ù���
 * �̳���ACharacter��֧���ƶ��Ͷ���
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

	// �������ö��󣨿�����Ĭ��ֵ�б༭��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Enemy Config")
	UEnemyConfig* EnemyConfig;

	// ������Χ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	USphereComponent* AttackRangeSphere;

	// ��ⷶΧ��������ڷ�����ң�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* DetectionSphere;

	// ������������������ײ�壩
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* InteractionTrigger;

	// ��ʾ UI ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractionWidget;

	APlayerController* CachedPlayerController = nullptr;

public:
	// ��ȡ��������
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	UEnemyConfig* GetEnemyConfig() const { return EnemyConfig; }

	// ���˺�������ͨ���¼����ĵ��ã�
	// ע�⣺��д����APawn��TakeDamage��ʹ�ò�ͬ�Ĳ���
	// Ϊ�˱����븸�ຯ����ͻ��ʹ��ReceiveDamage��Ϊ��Ҫ�ӿ�
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ReceiveDamage(int32 DamageAmount, AActor* DamageCauser = nullptr);

	// ��д����TakeDamage���������ڲ�����ReceiveDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ��������
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	// �ж��Ƿ�����
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsDead() const;

	// ��ȡ��ǰ�ܻ�״̬
	UFUNCTION(BlueprintCallable, Category = "Combat")
	EEnemyHitState GetHitState() const { return CurrentHitState; }

	// �ж��Ƿ����ܻ�Ӳֱ״̬
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInHitStun() const { return CurrentHitState == EEnemyHitState::Hit; }

	// �ж��Ƿ��޵�
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInvulnerable() const { return CurrentHitState == EEnemyHitState::Invulnerable; }
	
	// �Ƿ����ڹ�������������ͼ / AnimInstance ��ȡ��
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAttacking() const { return bIsAttacking; }

	// �Ƿ��������ܣ���������ͼ / AnimInstance ��ȡ��
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	bool IsDodging() const { return bIsDodging; }

	// ========== AIϵͳ��غ��� ==========
	
	// ��ȡ��ǰAI״̬
	UFUNCTION(BlueprintCallable, Category = "AI")
	EEnemyAIState GetAIState() const { return CurrentAIState; }

	// ����AI״̬���ڲ�ʹ�ã��ɱ���д��
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void SetAIState(EEnemyAIState NewState);

	// �ж��Ƿ��ڹ�����Χ��
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsPlayerInAttackRange() const;

	// �ж��Ƿ��⵽���
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsPlayerDetected() const;

	// ��ȡ��ҽ�ɫ����
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ACharacter* GetPlayerCharacter() const;

	// ========== ����ϵͳ��غ��� ==========
	
	/**
	 * ִ�й����ж����ɶ���֪ͨ���ã�
	 * ���ҹ�����Χ�ڵ�Ŀ�겢����˺�
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PerformAttack();

	/**
	 * ��ʼ���������Ź�������
	 * @return �Ƿ�ɹ���ʼ��������������δ���û���ȴ���򷵻�false��
	 */
	virtual bool StartAttack();

	/**
	 * ����Ƿ���Թ�������ȴʱ���Ƿ������
	 */
	virtual bool CanAttack() const;

	// ========== ����ϵͳ��غ��� ==========
	
	/**
	 * �ж��Ƿ��������
	 */
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	bool CanDodge() const;

	/**
	 * �ж��Ƿ�Ӧ�����ܣ����ھ���͸��ʣ�
	 */
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	bool ShouldDodge() const;

	/**
	 * ִ�����ܣ��������ܶ�����ִ�������ƶ�
	 */
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void PerformDodge();

	/**
	 * �������Ƿ����ڹ���
	 */
	bool IsPlayerAttacking() const;

protected:
	// �Ƿ�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsDead = false;

	// ��ǰ�ܻ�״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	EEnemyHitState CurrentHitState = EEnemyHitState::Normal;

	// �ܻ�Ӳֱʱ�䣨�룩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HitStunDuration = 0.3f;

	// �ܻ����޵�ʱ�䣨�룩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InvulnerableDuration = 0.5f;

	// �ܻ�Ӳֱ��ʱ��
	FTimerHandle HitStunTimerHandle;

	// �޵�״̬��ʱ��
	FTimerHandle InvulnerableTimerHandle;

	// ========== AIϵͳ��س�Ա ==========
	
	// ��ǰAI״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EEnemyAIState CurrentAIState = EEnemyAIState::Idle;

	// ��ҽ�ɫ���ã����棬����ÿ֡���ң�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class ACharacter* PlayerCharacter = nullptr;

	// AI���¼�����룩����Ҫÿ֡���£��Ż�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AIUpdateInterval = 0.1f;

	// AI���¼�ʱ��
	float AIUpdateTimer = 0.0f;

	// ��ʼ��ռλ��ģ�ͣ�Mannequin��
	void InitializePlaceholderMesh();

	// �����ܻ�Ӳֱ״̬
	void EnterHitStun();

	// �����ܻ�Ӳֱ״̬
	void EndHitStun();

	// �����޵�״̬
	void EnterInvulnerable();

	// �����޵�״̬
	void EndInvulnerable();

	// ��ȡ�¼����ģ�����֪ͨ����ֵ�仯�ȣ�
	class UEventCenter* GetEventCenter() const;


	/**
	 * ��ȡ������Χ�ڵĿɹ���Ŀ�꣨���ݱ�ǩ��Ĭ�ϲ���Player��ǩ��
	 * �ο���ɫ���GetNearbyObjectsWithTagʵ��
	 * @param AttackRange ������Χ�뾶�����<=0��ʹ��AttackRangeSphere�İ뾶��
	 * @return ������Χ�ڵĿɹ���Ŀ���б���Ĭ�ϲ���"Player"��ǩ��
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	TArray<AActor*> GetAttackTargetsInRange(float AttackRange = 0.0f) const;

	/**
	 * ��ȡ������Χ�ڵĿɹ���Ŀ�꣨��չ�汾��C++�ڲ�ʹ�ã�
	 * @param AttackRange ������Χ�뾶�����<=0��ʹ��AttackRangeSphere�İ뾶��
	 * @param TargetTags Ŀ���ǩ�б�
	 * @return ������Χ�ڵĿɹ���Ŀ���б�
	 */
	TArray<AActor*> GetAttackTargetsInRangeWithTags(float AttackRange, const TArray<FName>& TargetTags) const;

	// ========== AIϵͳ�ڲ����� ==========
	
	// ����AI״̬����Tick�е��ã�
	virtual void UpdateAI(float DeltaTime);

	// �����ң���DetectionSphere��Χ�ڣ�
	// ע�⣺�˷���������PlayerCharacter���ã����Բ���const
	virtual bool DetectPlayer();

	// ���㵽��ҵľ���
	float GetDistanceToPlayer() const;

	// ִ��׷���ƶ�
	virtual void ChasePlayer(float DeltaTime);

	// ֹͣ�ƶ�
	void StopMovement();

	// ״̬�л��߼������ݵ�ǰ���������һ��״̬
	virtual EEnemyAIState DetermineNextState() const;

	/**
	 * ������̫������ص�
	 */
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// �ص���ʼʱ�ص�
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// �ص�����ʱ�ص�
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// ========== ����ϵͳ��س�Ա ==========
	
	// ����������̫��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* AttackMontage;

	// �Ƿ����ڹ��������ڷ�ֹ�ظ��ж���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAttacking = false;

	// ������ȴ��ʱ��
	FTimerHandle AttackCooldownTimer;

	// ��ǰ������ȴʣ��ʱ�䣨���ڵ��Ժ�UI��ʾ��
	float AttackCooldownRemaining = 0.0f;

	// ���ι��������������е�Ŀ�꣨��ֹͬһ�����������ظ��ж���
	UPROPERTY()
	TArray<AActor*> AlreadyHitTargetsInThisAttack;

	// ========== �ܻ�������������̫�� ==========
	
	// �ܻ�������̫��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* HitMontage;

	// ����������̫��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* DeathMontage;

	// ========== ����ϵͳ��س�Ա ==========
	
	// ���ܶ�����̫��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	class UAnimMontage* DodgeMontage;

	// �Ƿ���������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dodge")
	bool bIsDodging = false;

	// �����ڼ��Ƿ��޵�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dodge")
	bool bInvulnerableDuringDodge = false;

	// ���ܸ��ʣ�0.0-1.0��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeProbability = 0.5f;

	// �����ҹ����ķ�Χ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeRange = 300.0f;

	// ����ǿ�ȣ��ƶ��ٶȣ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeStrength = 800.0f;

	// ���ܳ���ʱ�䣨�룩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeDuration = 0.3f;

	// ������ȴʱ�䣨�룩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeCooldown = 2.0f;

	// ���ܶ�ʱ��
	FTimerHandle DodgeTimerHandle;

	// ������ȴ��ʱ��
	FTimerHandle DodgeCooldownTimerHandle;

	// ������̫������ص�
	UFUNCTION()
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// �������ܣ����ø��ࣩ
	void EndDodge();

	// ������ȴ�����ص�
	void OnDodgeCooldownEnd();
};
