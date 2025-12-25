// Fill out your copyright notice in the Description page of Project Settings.

#include "BossEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "EventCenter.h"
#include "black_money/black_moneyCharacter.h"

ABossEnemy::ABossEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Boss初始化：设置碰撞体大小、较大的检测范围
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 120.0f);
	
	// 设置移动速度，Boss移动速度较慢
	GetCharacterMovement()->MaxWalkSpeed = 120.0f;
	
	// 设置父类的攻击范围
	if (AttackRangeSphere)
	{
		AttackRangeSphere->SetSphereRadius(200.0f);
	}
	
	// 设置父类的检测范围
	if (DetectionSphere)
	{
		DetectionSphere->SetSphereRadius(1500.0f);
	}

	// 初始化阶段系统
	CurrentPhase = EBossPhase::Phase1;
	Phase1AttackMontage = nullptr;
	Phase2AttackMontage = nullptr;
	RoarMontage = nullptr;
	Phase2ParticleEffect = nullptr;
	Phase2AttackMultiplier = 1.5f;
	HealthRegenRate = 5.0f;

	// 初始化怒气值系统
	CurrentRageValue = 0.0f;
	MaxRageValue = 100.0f;
	RagePerDamage = 1.0f;

	// 初始化技能系统
	SkillWindupMontage = nullptr;
	SkillAttackMontage = nullptr;
	bIsUsingSkill = false;
	SkillDamage = 50;
	SkillAttackRange = 300.0f;
	SkillCooldown = 10.0f;
	SkillCooldownRemaining = 0.0f;

	// 配置闪避系统，Boss不闪避，将闪避概率设为0
	DodgeProbability = 0.0f;      // 0%闪避概率，Boss不闪避
	DodgeRange = 300.0f;           // 闪避玩家的范围
	DodgeStrength = 800.0f;       // 闪避时的移动力度
	DodgeDuration = 0.3f;         // 闪避动画持续时间
	DodgeCooldown = 2.0f;         // 闪避冷却时间

	// 创建第二阶段粒子效果组件
	Phase2ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Phase2ParticleComponent"));
	Phase2ParticleComponent->SetupAttachment(RootComponent);
	Phase2ParticleComponent->bAutoActivate = false; // 初始不激活，等待进入第二阶段
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 配置Boss的EnemyConfig属性
	if (EnemyConfig)
	{
		// Boss属性：高血量、高攻击力、高防御
		EnemyConfig->MaxHp = 500;
		EnemyConfig->CurrentHp = 500;
		EnemyConfig->Attack = 25;      // 高攻击力：第一阶段25点，第二阶段37.5点（约38点）
		EnemyConfig->Defence = 10;     // 高防御力

		// 配置移动速度和攻击速度，以及攻击范围
		EnemyConfig->MoveSpeed = 120.0f;       // 较慢的移动速度
		EnemyConfig->AttackSpeed = 0.8f;       // 每秒1.25次攻击
		EnemyConfig->AttackRange = 200.0f;     // 较大的攻击范围
		EnemyConfig->DetectionRange = 1500.0f; // 很大的检测范围

		// 初始化EnemyConfig，确保当前血量不超过最大血量
		EnemyConfig->Initialize();
	}

	// 同步移动速度到角色移动组件
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = EnemyConfig ? EnemyConfig->MoveSpeed : 120.0f;
	}

	// 配置第二阶段粒子效果资源
	if (Phase2ParticleComponent && Phase2ParticleEffect)
	{
		Phase2ParticleComponent->SetTemplate(Phase2ParticleEffect);
		// 粒子效果初始不激活，等待进入第二阶段
	}

	// 初始化阶段
	CurrentPhase = EBossPhase::Phase1;
}

void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 更新技能冷却时间
	if (SkillCooldownRemaining > 0.0f)
	{
		SkillCooldownRemaining = FMath::Max(0.0f, SkillCooldownRemaining - DeltaTime);
	}

	// 检查阶段转换
	CheckPhaseTransition();

	// 检查是否可以释放技能（怒气值满且不在冷却）
	if (CanUseSkill() && !bIsUsingSkill && !bIsAttacking && CurrentAIState != EEnemyAIState::Hit)
	{
		UseSkill();
	}
}

void ABossEnemy::ReceiveDamage(int32 DamageAmount, AActor* DamageCauser)
{
	// 调用父类的伤害处理
	Super::ReceiveDamage(DamageAmount, DamageCauser);

	// 如果已经死亡或处于无敌状态，不增加怒气
	if (bIsDead || CurrentHitState == EEnemyHitState::Invulnerable)
	{
		// 如果死亡，停止第二阶段粒子效果
		if (bIsDead && Phase2ParticleComponent && Phase2ParticleComponent->IsActive())
		{
			Phase2ParticleComponent->Deactivate();
			UE_LOG(LogTemp, Log, TEXT("Boss %s died, Phase 2 particle effect deactivated"), *GetName());
		}
		return;
	}

	// 增加怒气值（每次受到伤害时）
	float RageGain = DamageAmount * RagePerDamage;
	AddRageValue(RageGain);

	UE_LOG(LogTemp, Log, TEXT("Boss %s gained %.2f rage (Total: %.2f/%.2f)"), 
		*GetName(), RageGain, CurrentRageValue, MaxRageValue);
}

void ABossEnemy::PerformAttack()
{
	// 如果已经死亡、正在受击硬直、正在使用技能，不执行攻击
	if (bIsDead || CurrentHitState == EEnemyHitState::Hit || bIsUsingSkill)
	{
		return;
	}

	// 防止同一攻击重复判定
	if (bIsAttacking && AlreadyHitTargetsInThisAttack.Num() > 0)
	{
		return;
	}

	// 获取事件中心
	UEventCenter* EventCenter = GetEventCenter();
	if (!EventCenter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss PerformAttack: EventCenter is null!"));
		return;
	}

	// 获取攻击范围内的目标
	TArray<AActor*> Targets = GetAttackTargetsInRange();
	if (Targets.Num() == 0)
	{
		return;
	}

	// 获取当前阶段的攻击伤害
	int32 AttackDamage = GetCurrentPhaseAttackDamage();

	// 对每个目标造成伤害
	for (AActor* Target : Targets)
	{
		if (AlreadyHitTargetsInThisAttack.Contains(Target))
		{
			continue;
		}

		AlreadyHitTargetsInThisAttack.Add(Target);

		// 通过事件中心造成伤害
		float ActualDamage = EventCenter->MakeDamage(
			Target,
			static_cast<float>(AttackDamage),
			GetController(),
			this
		);

		UE_LOG(LogTemp, Log, TEXT("Boss %s attacked %s, dealt %f damage (Phase %d)"), 
			*GetName(), *Target->GetName(), ActualDamage, (int32)CurrentPhase);
	}
}

bool ABossEnemy::StartAttack()
{
	// 如果正在使用技能，不能开始普通攻击
	if (bIsUsingSkill)
	{
		return false;
	}

	// 获取当前阶段的攻击蒙太奇
	UAnimMontage* CurrentAttackMontage = GetCurrentPhaseAttackMontage();
	if (!CurrentAttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss StartAttack: No attack montage for current phase!"));
		return false;
	}

	// 检查是否可以攻击
	if (!CanAttack())
	{
		return false;
	}

	// 获取动画实例
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss StartAttack: AnimInstance is null!"));
		return false;
	}

	// 设置攻击状态
	bIsAttacking = true;
	AlreadyHitTargetsInThisAttack.Empty();

	// 播放当前阶段的攻击蒙太奇
	float PlayRate = EnemyConfig ? EnemyConfig->AttackSpeed : 1.0f;
	float PlayTime = AnimInstance->Montage_Play(CurrentAttackMontage, PlayRate);

	if (PlayTime > 0.0f)
	{
		// 绑定蒙太奇结束委托，使用lambda包装protected成员函数
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			this->OnAttackMontageEnded(Montage, bInterrupted);
		});
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, CurrentAttackMontage);

		// 更新攻击冷却时间
		AttackCooldownRemaining = 1.0f / (EnemyConfig ? EnemyConfig->AttackSpeed : 1.0f);

		UE_LOG(LogTemp, Log, TEXT("Boss %s started attack (Phase %d)"), *GetName(), (int32)CurrentPhase);
		return true;
	}

	bIsAttacking = false;
	return false;
}

void ABossEnemy::CheckPhaseTransition()
{
	// 如果已经在第二阶段或阶段转换中，不需要检查
	if (CurrentPhase == EBossPhase::Phase2 || CurrentPhase == EBossPhase::PhaseTransition)
	{
		return;
	}

	// 检查血量是否低于50%
	if (EnemyConfig && EnemyConfig->GetHealthPercentage() <= 0.5f)
	{
		EnterPhase2();
	}
}

void ABossEnemy::EnterPhase2()
{
	if (CurrentPhase == EBossPhase::Phase2)
	{
		return; // 已经在第二阶段
	}

	UE_LOG(LogTemp, Warning, TEXT("Boss %s entering Phase 2!"), *GetName());

	// 设置阶段为转换中
	CurrentPhase = EBossPhase::PhaseTransition;

	// 停止当前的所有动作
	StopMovement();
	if (bIsAttacking)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			UAnimMontage* CurrentMontage = GetCurrentPhaseAttackMontage();
			if (CurrentMontage && AnimInstance->Montage_IsPlaying(CurrentMontage))
			{
				AnimInstance->Montage_Stop(0.2f, CurrentMontage);
			}
		}
		bIsAttacking = false;
	}

	// 播放咆哮动画
	PlayRoarAnimation();
}

void ABossEnemy::PlayRoarAnimation()
{
	if (!RoarMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss %s: RoarMontage is null, skipping to Phase 2"), *GetName());
		// 如果没有咆哮动画，直接进入第二阶段
		CurrentPhase = EBossPhase::Phase2;
		StartHealthRegen();
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss %s: AnimInstance is null"), *GetName());
		CurrentPhase = EBossPhase::Phase2;
		StartHealthRegen();
		return;
	}

	// 播放咆哮动画
	float PlayTime = AnimInstance->Montage_Play(RoarMontage, 1.0f);
	if (PlayTime > 0.0f)
	{
		// 绑定蒙太奇结束委托
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &ABossEnemy::OnRoarMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, RoarMontage);

		UE_LOG(LogTemp, Log, TEXT("Boss %s playing roar animation"), *GetName());
	}
	else
	{
		// 播放失败，直接进入第二阶段
		CurrentPhase = EBossPhase::Phase2;
		StartHealthRegen();
	}
}

void ABossEnemy::OnRoarMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 咆哮动画结束，进入第二阶段
	CurrentPhase = EBossPhase::Phase2;
	StartHealthRegen();

	// 激活第二阶段粒子效果（持续播放）
	if (Phase2ParticleComponent)
	{
		Phase2ParticleComponent->Activate();
		UE_LOG(LogTemp, Log, TEXT("Boss %s Phase 2 particle effect activated"), *GetName());
	}
	else if (Phase2ParticleEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss %s Phase2ParticleComponent is null, cannot activate particle effect"), *GetName());
	}

	UE_LOG(LogTemp, Log, TEXT("Boss %s roar animation ended, now in Phase 2"), *GetName());
}

void ABossEnemy::AddRageValue(float Amount)
{
	if (Amount <= 0.0f || bIsDead)
	{
		return;
	}

	CurrentRageValue = FMath::Min(MaxRageValue, CurrentRageValue + Amount);
}

bool ABossEnemy::CanUseSkill() const
{
	// 需要怒气值满、技能冷却结束才能使用技能，不能受击硬直
	return CurrentRageValue >= MaxRageValue 
		&& SkillCooldownRemaining <= 0.0f 
		&& !bIsUsingSkill 
		&& CurrentHitState != EEnemyHitState::Hit
		&& !bIsDead;
}

void ABossEnemy::UseSkill()
{
	if (!CanUseSkill() || !SkillWindupMontage)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Boss %s using skill (Jump Attack)!"), *GetName());

	// 设置技能状态
	bIsUsingSkill = true;
	CurrentRageValue = 0.0f; // 清空怒气值

	// 停止当前的所有动作
	StopMovement();
	if (bIsAttacking)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			UAnimMontage* CurrentMontage = GetCurrentPhaseAttackMontage();
			if (CurrentMontage && AnimInstance->Montage_IsPlaying(CurrentMontage))
			{
				AnimInstance->Montage_Stop(0.2f, CurrentMontage);
			}
		}
		bIsAttacking = false;
	}

	// 开始技能冷却（前摇阶段就开始冷却）
	SkillCooldownRemaining = SkillCooldown;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			SkillCooldownTimerHandle,
			this,
			&ABossEnemy::OnSkillCooldownEnd,
			SkillCooldown,
			false
		);
	}

	// 播放技能前摇动画
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		float PlayTime = AnimInstance->Montage_Play(SkillWindupMontage, 1.0f);
		if (PlayTime > 0.0f)
		{
			// 绑定前摇蒙太奇结束委托
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &ABossEnemy::OnSkillWindupMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillWindupMontage);

			UE_LOG(LogTemp, Log, TEXT("Boss %s playing skill windup animation"), *GetName());
		}
		else
		{
			// 播放失败，重置状态
			bIsUsingSkill = false;
			SkillCooldownRemaining = 0.0f;
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().ClearTimer(SkillCooldownTimerHandle);
			}
		}
	}
}

void ABossEnemy::OnSkillWindupMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		// 前摇被中断，重置状态
		bIsUsingSkill = false;
		SkillCooldownRemaining = 0.0f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(SkillCooldownTimerHandle);
		}
		UE_LOG(LogTemp, Warning, TEXT("Boss %s skill windup was interrupted"), *GetName());
		return;
	}

	// 前摇结束，播放技能攻击动画
	if (!SkillAttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss %s: SkillAttackMontage is null, ending skill"), *GetName());
		bIsUsingSkill = false;
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		float PlayTime = AnimInstance->Montage_Play(SkillAttackMontage, 1.0f);
		if (PlayTime > 0.0f)
		{
			// 绑定攻击蒙太奇结束委托
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &ABossEnemy::OnSkillAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillAttackMontage);

			UE_LOG(LogTemp, Log, TEXT("Boss %s playing skill attack animation"), *GetName());
		}
		else
		{
			// 播放失败，重置状态
			bIsUsingSkill = false;
		}
	}
}

void ABossEnemy::OnSkillAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsUsingSkill = false;
	UE_LOG(LogTemp, Log, TEXT("Boss %s skill attack montage ended"), *GetName());
}

void ABossEnemy::PerformSkillAttack()
{
	// 获取事件中心
	UEventCenter* EventCenter = GetEventCenter();
	if (!EventCenter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss PerformSkillAttack: EventCenter is null!"));
		return;
	}

	// 获取玩家位置
	ACharacter* Player = GetPlayerCharacter();
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss PerformSkillAttack: Player not found!"));
		return;
	}

	// 获取技能攻击范围内的所有目标
	FVector BossLocation = GetActorLocation();
	TArray<AActor*> Targets = GetAttackTargetsInRange(SkillAttackRange);

	// 对范围内的所有目标造成技能伤害
	for (AActor* Target : Targets)
	{
		float ActualDamage = EventCenter->MakeDamage(
			Target,
			static_cast<float>(SkillDamage),
			GetController(),
			this
		);

		UE_LOG(LogTemp, Log, TEXT("Boss %s skill attack hit %s, dealt %f damage"), 
			*GetName(), *Target->GetName(), ActualDamage);
	}
}

void ABossEnemy::OnSkillCooldownEnd()
{
	SkillCooldownRemaining = 0.0f;
	UE_LOG(LogTemp, Log, TEXT("Boss %s skill cooldown ended"), *GetName());
}

void ABossEnemy::StartHealthRegen()
{
	if (CurrentPhase != EBossPhase::Phase2)
	{
		return;
	}

	// 每秒执行一次生命回复
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			HealthRegenTimerHandle,
			this,
			&ABossEnemy::TickHealthRegen,
			1.0f, // 每秒执行一次
			true  // 循环
		);
	}

	UE_LOG(LogTemp, Log, TEXT("Boss %s started health regeneration"), *GetName());
}

void ABossEnemy::StopHealthRegen()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
	}
}

void ABossEnemy::TickHealthRegen()
{
	if (CurrentPhase != EBossPhase::Phase2 || !EnemyConfig || bIsDead)
	{
		StopHealthRegen();
		return;
	}

	// 回复生命值
	int32 OldHp = EnemyConfig->CurrentHp;
	EnemyConfig->CurrentHp = FMath::Min(EnemyConfig->MaxHp, EnemyConfig->CurrentHp + static_cast<int32>(HealthRegenRate));

	if (EnemyConfig->CurrentHp > OldHp)
	{
		UE_LOG(LogTemp, Log, TEXT("Boss %s regenerated %d HP (Current: %d/%d)"), 
			*GetName(), EnemyConfig->CurrentHp - OldHp, EnemyConfig->CurrentHp, EnemyConfig->MaxHp);
	}
}

void ABossEnemy::UpdateAI(float DeltaTime)
{
	// 如果正在使用技能或在阶段转换中，不更新AI
	if (bIsUsingSkill || CurrentPhase == EBossPhase::PhaseTransition)
	{
		return;
	}

	// 调用父类的AI更新
	Super::UpdateAI(DeltaTime);
}

UAnimMontage* ABossEnemy::GetCurrentPhaseAttackMontage() const
{
	switch (CurrentPhase)
	{
	case EBossPhase::Phase1:
		return Phase1AttackMontage;
	case EBossPhase::Phase2:
		return Phase2AttackMontage;
	default:
		return Phase1AttackMontage; // 默认返回第一阶段
	}
}

int32 ABossEnemy::GetCurrentPhaseAttackDamage() const
{
	int32 BaseAttack = EnemyConfig ? EnemyConfig->Attack : 10;
	
	if (CurrentPhase == EBossPhase::Phase2)
	{
		return static_cast<int32>(BaseAttack * Phase2AttackMultiplier);
	}
	
	return BaseAttack;
}

void ABossEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 调用父类的处理逻辑
	Super::OnAttackMontageEnded(Montage, bInterrupted);
}

bool ABossEnemy::CanAttack() const
{
	// 先调用父类的检查（检查死亡、受击状态、冷却等）
	// 但需要跳过AttackMontage的检查，因为Boss使用Phase1AttackMontage和Phase2AttackMontage
	// 所以我们手动检查这些条件
	
	// 检查死亡和受击状态
	if (bIsDead || CurrentHitState == EEnemyHitState::Hit)
	{
		return false;
	}

	// 检查攻击冷却
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(AttackCooldownTimer))
	{
		return false;
	}

	// Boss特殊检查：如果正在使用技能，不能攻击
	if (bIsUsingSkill)
	{
		return false;
	}

	// 检查当前阶段的攻击蒙太奇是否存在
	UAnimMontage* CurrentAttackMontage = GetCurrentPhaseAttackMontage();
	if (!CurrentAttackMontage)
	{
		return false;
	}

	return true;
}
