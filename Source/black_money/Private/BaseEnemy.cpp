// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "EventCenter.h"
#include "black_moneyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "UIEnemyHp.h"
#include "black_money/black_moneyCharacter.h"
#include "ToolHp.h"
#include "ToolMp.h"

ABaseEnemy::ABaseEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 设置碰撞胶囊体大小
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// 不随控制器旋转而旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 配置角色移动组件
	GetCharacterMovement()->bOrientRotationToMovement = false; // 改为false，手动控制朝向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1080.0f, 0.0f); // 提高转向速度（从500改为1080度/秒）
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 创建攻击范围检测组件
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);
	AttackRangeSphere->SetSphereRadius(150.0f);
	AttackRangeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeSphere->SetHiddenInGame(true);

	// 创建检测范围组件（用于发现玩家）
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(1000.0f);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DetectionSphere->SetHiddenInGame(true);

	// 创建交互触发器（球形）
	InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetupAttachment(RootComponent);
	InteractionTrigger->SetSphereRadius(700.0f);
	InteractionTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	// 绑定重叠事件（必须在构造函数中绑定！）
	InteractionTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseEnemy::OnOverlapBegin);
	InteractionTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &ABaseEnemy::OnOverlapEnd);



	// 创建 WidgetComponent（必须在构造函数中创建）
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetWidgetClass(UUIEnemyHp::StaticClass());
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::World); // 世界空间
	InteractionWidget->SetDrawSize(FVector2D(200.0f, 50.0f));
	InteractionWidget->SetPivot(FVector2D(0.5f, 0.0f)); // 底部居中对齐
	InteractionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // 在物体上方 100 单位
	InteractionWidget->SetVisibility(false); // 初始隐藏

	// 初始化占位符模型
	InitializePlaceholderMesh();

	// 创建怪物配置对象（UObject使用NewObject，在BeginPlay中初始化）
	EnemyConfig = nullptr; // 在BeginPlay中创建

	// 设置默认标签
	Tags.Add(FName("Enemy"));

	// 初始化攻击系统相关变量
	AttackMontage = nullptr;
	bIsAttacking = false;
	AttackCooldownRemaining = 0.0f;

	// 初始化受击和死亡动画蒙太奇
	HitMontage = nullptr;
	DeathMontage = nullptr;

	// 初始化闪避系统相关变量
	DodgeMontage = nullptr;
	bIsDodging = false;
	bInvulnerableDuringDodge = false;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 创建并初始化怪物配置对象
	// 如果类默认值中没有创建，则在运行时创建
	if (!EnemyConfig)
	{
		EnemyConfig = NewObject<UEnemyConfig>(this);
	}
	// 确保配置对象已初始化（无论是否在类默认值中创建）
	if (EnemyConfig)
	{
		EnemyConfig->Initialize();
	}

	// 初始化AI状态
	CurrentAIState = EEnemyAIState::Idle;
	AIUpdateTimer = 0.0f;

	// 尝试获取玩家角色引用
	PlayerCharacter = GetPlayerCharacter();
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Cast<UUIEnemyHp>(InteractionWidget->GetUserWidgetObject())->_hp = EnemyConfig->CurrentHp;
	// 如果已死亡，不更新AI和移动
	if (bIsDead || CurrentAIState == EEnemyAIState::Dead)
	{
		return;
	}

	// 如果处于Chase状态，每帧都执行移动（确保移动连续）
	// 注意：Attack状态不应该在这里移动，因为SetAIState(Attack)时会调用StopMovement
	if (CurrentAIState == EEnemyAIState::Chase && !bIsDead)
	{
		ChasePlayer(DeltaTime);
	}
	
	// 如果处于Attack状态，确保停止移动（防止ChasePlayer在状态切换前添加的移动输入）
	if (CurrentAIState == EEnemyAIState::Attack && !bIsDead)
	{
		// 攻击状态下，持续停止移动，确保不会因为残留的移动输入而移动
		if (GetCharacterMovement() && GetCharacterMovement()->Velocity.Size() > 0.1f)
		{
			StopMovement();
		}
		
		// 攻击状态下，仍然朝向玩家（但不移动）
		if (PlayerCharacter && IsValid(PlayerCharacter))
		{
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector EnemyLocation = GetActorLocation();
			FVector Direction = PlayerLocation - EnemyLocation;
			Direction.Z = 0.0f;
			Direction.Normalize();
			
			FRotator TargetRotation = Direction.Rotation();
			FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
			SetActorRotation(NewRotation);
		}
	}

	// 累积AI更新计时器
	AIUpdateTimer += DeltaTime;

	// 按间隔更新AI（优化性能，状态决策不需要每帧执行）
	if (AIUpdateTimer >= AIUpdateInterval)
	{
		UpdateAI(DeltaTime);
		AIUpdateTimer = 0.0f;
	}
}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// 将float转换为int32，调用我们的ReceiveDamage方法
	ReceiveDamage(static_cast<int32>(DamageAmount), DamageCauser);
	return DamageAmount;
}

void ABaseEnemy::ReceiveDamage(int32 DamageAmount, AActor* DamageCauser)
{
	// 如果已死亡或处于无敌状态，不处理伤害
	if (bIsDead || !EnemyConfig || CurrentHitState == EEnemyHitState::Invulnerable)
	{
		return;
	}

	// 如果正在闪避且处于无敌状态，忽略伤害
	if (bInvulnerableDuringDodge)
	{
		UE_LOG(LogTemp, Log, TEXT("Enemy %s: Damage blocked by dodge i-frame"), *GetName());
		return;
	}

	// 计算实际伤害（考虑防御力）
	int32 ActualDamage = FMath::Max(1, DamageAmount - EnemyConfig->Defence);
	
	// 减少生命值
	const int32 OldHp = EnemyConfig->CurrentHp;
	EnemyConfig->CurrentHp = FMath::Max(0, EnemyConfig->CurrentHp - ActualDamage);

	UE_LOG(LogTemp, Log, TEXT("Enemy %s took %d damage (from %d), remaining HP: %d"), 
		*GetName(), ActualDamage, OldHp, EnemyConfig->CurrentHp);

	// 通知事件中心生命值变化（如果需要）
	// 可以通过事件中心广播生命值变化事件给UI系统等

	// 进入受击硬直状态
	EnterHitStun();

	// 检查是否死亡
	if (EnemyConfig->IsDead())
	{
		Die();
	}
}

void ABaseEnemy::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	UE_LOG(LogTemp, Log, TEXT("Enemy %s died"), *GetName());

	// 切换到死亡AI状态
	SetAIState(EEnemyAIState::Dead);

	// 通过事件中心通知死亡事件
	if (UEventCenter* EventCenter = GetEventCenter())
	{
		// 可以通过事件中心广播死亡事件
		// 例如：通知UI更新、掉落物品、播放音效等
	}

	// 清除所有计时器
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HitStunTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(InvulnerableTimerHandle);
	}

	// 禁用碰撞
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 停止移动
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	// 停止所有正在播放的动画蒙太奇
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 停止攻击动画
		if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			AnimInstance->Montage_Stop(0.0f, AttackMontage);
		}
		
		// 停止受击动画
		if (HitMontage && AnimInstance->Montage_IsPlaying(HitMontage))
		{
			AnimInstance->Montage_Stop(0.0f, HitMontage);
		}
		
		// 播放死亡动画
		if (DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Enemy %s playing death animation"), *GetName());
		}
	}

	// 掉落道具：随机从 ToolHp 和 ToolMp 中选择一个
	if (GetWorld())
	{
		// 获取怪物死亡位置
		FVector DeathLocation = GetActorLocation();
		FRotator DeathRotation = FRotator::ZeroRotator;
		
		// 随机选择道具类型（0或1）
		int32 RandomChoice = FMath::RandRange(0, 1);
		
		AActor* SpawnedTool = nullptr;
		if (RandomChoice == 0)
		{
			// 生成 ToolHp（回血道具）
			SpawnedTool = GetWorld()->SpawnActor<AToolHp>(DeathLocation, DeathRotation);
			UE_LOG(LogTemp, Log, TEXT("Enemy %s dropped ToolHp at location %s"), *GetName(), *DeathLocation.ToString());
		}
		else
		{
			// 生成 ToolMp（回蓝道具）
			SpawnedTool = GetWorld()->SpawnActor<AToolMp>(DeathLocation, DeathRotation);
			UE_LOG(LogTemp, Log, TEXT("Enemy %s dropped ToolMp at location %s"), *GetName(), *DeathLocation.ToString());
		}
		
		// 检查是否成功生成道具
		if (!SpawnedTool)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn tool at enemy death location for %s"), *GetName());
		}
	}

	// TODO: 延迟销毁或播放死亡效果（可以添加延迟后调用Destroy()）
}

bool ABaseEnemy::IsDead() const
{
	return bIsDead || (EnemyConfig && EnemyConfig->IsDead());
}

void ABaseEnemy::InitializePlaceholderMesh()
{
	// 使用Mannequin作为占位符模型
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannequinMeshFinder(
		TEXT("/Engine/EngineMeshes/SkeletalMesh/SK_Mannequin.SK_Mannequin")
	);

	if (MannequinMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MannequinMeshFinder.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		
		UE_LOG(LogTemp, Log, TEXT("BaseEnemy: Successfully loaded Mannequin placeholder mesh"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseEnemy: Failed to load Mannequin placeholder mesh"));
	}
}

void ABaseEnemy::EnterHitStun()
{
	if (CurrentHitState == EEnemyHitState::Hit)
	{
		return; // 已经在硬直状态
	}

	CurrentHitState = EEnemyHitState::Hit;

	// 切换到受击AI状态
	SetAIState(EEnemyAIState::Hit);

	// 停止移动
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	// 设置硬直计时器
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			HitStunTimerHandle,
			this,
			&ABaseEnemy::EndHitStun,
			HitStunDuration,
			false
		);
	}

	// 播放受击动画
	if (HitMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 停止当前播放的攻击动画（如果正在攻击）
			if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				AnimInstance->Montage_Stop(0.2f, AttackMontage);
			}
			
			// 播放受击动画
			AnimInstance->Montage_Play(HitMontage, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Enemy %s playing hit animation"), *GetName());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Enemy %s entered hit stun for %.2f seconds"), *GetName(), HitStunDuration);
}

void ABaseEnemy::EndHitStun()
{
	if (CurrentHitState != EEnemyHitState::Hit)
	{
		return;
	}

	CurrentHitState = EEnemyHitState::Normal;

	// 进入短暂无敌状态
	EnterInvulnerable();

	UE_LOG(LogTemp, Log, TEXT("Enemy %s ended hit stun"), *GetName());
}

void ABaseEnemy::EnterInvulnerable()
{
	CurrentHitState = EEnemyHitState::Invulnerable;

	// 设置无敌计时器
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			InvulnerableTimerHandle,
			this,
			&ABaseEnemy::EndInvulnerable,
			InvulnerableDuration,
			false
		);
	}

	UE_LOG(LogTemp, Log, TEXT("Enemy %s entered invulnerable state for %.2f seconds"), *GetName(), InvulnerableDuration);
}

void ABaseEnemy::EndInvulnerable()
{
	if (CurrentHitState != EEnemyHitState::Invulnerable)
	{
		return;
	}

	CurrentHitState = EEnemyHitState::Normal;

	UE_LOG(LogTemp, Log, TEXT("Enemy %s ended invulnerable state"), *GetName());
}

UEventCenter* ABaseEnemy::GetEventCenter() const
{
	if (UWorld* World = GetWorld())
	{
		if (Ublack_moneyGameInstance* GameInstance = Cast<Ublack_moneyGameInstance>(World->GetGameInstance()))
		{
			return Cast<UEventCenter>(GameInstance->GetEventCenter());
		}
	}
	return nullptr;
}

TArray<AActor*> ABaseEnemy::GetAttackTargetsInRange(float AttackRange) const
{
	// 默认查找"Player"标签
	TArray<FName> DefaultTags;
	DefaultTags.Add(FName("Player"));
	return GetAttackTargetsInRangeWithTags(AttackRange, DefaultTags);
}

TArray<AActor*> ABaseEnemy::GetAttackTargetsInRangeWithTags(float AttackRange, const TArray<FName>& TargetTags) const
{
	TArray<AActor*> Result;

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetAttackTargetsInRangeWithTags: No valid world!"));
		return Result;
	}

	// 如果没有指定范围，使用AttackRangeSphere的半径
	float ActualRange = AttackRange;
	if (ActualRange <= 0.0f && AttackRangeSphere)
	{
		ActualRange = AttackRangeSphere->GetScaledSphereRadius();
	}
	
	// 如果还是没有有效范围，使用配置中的攻击范围
	if (ActualRange <= 0.0f && EnemyConfig)
	{
		ActualRange = EnemyConfig->AttackRange;
	}

	// 如果还是没有，使用默认值
	if (ActualRange <= 0.0f)
	{
		ActualRange = 150.0f; // 默认攻击范围
	}

	// 使用怪物前方的位置作为检测中心（而不是怪物中心）
	// 这样攻击检测更符合实际攻击方向
	FVector EnemyLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector SphereCenter = EnemyLocation + ForwardVector * (ActualRange * 0.5f); // 检测中心在怪物前方一半攻击距离处

	// 设置检测的对象类型（参考Character类的实现）
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // 主要检测Pawn类型（玩家和怪物）

	// 执行球形范围检测
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(
		World,
		SphereCenter,
		ActualRange * 0.5f, // 使用一半范围，因为检测中心已经前移了
		ObjectTypes,
		AActor::StaticClass(),  // 查找所有Actor
		TArray<AActor*>(),      // 忽略列表（空，不忽略任何对象）
		OverlappingActors
	);

	// 筛选出具有指定标签的Actor，并检查是否在攻击范围内（方向性检查）
	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || Actor == this) // 排除自身
		{
			continue;
		}

		// 检查是否具有目标标签
		bool bHasTargetTag = false;
		for (const FName& TagName : TargetTags)
		{
			if (Actor->ActorHasTag(TagName))
			{
				bHasTargetTag = true;
				break;
			}
		}

		if (!bHasTargetTag)
		{
			continue;
		}

		// 方向性检查：确保目标在怪物前方（180度扇形范围内）
		FVector ToTarget = Actor->GetActorLocation() - EnemyLocation;
		ToTarget.Z = 0.0f; // 忽略高度差
		ToTarget.Normalize();
		
		FVector Forward = ForwardVector;
		Forward.Z = 0.0f;
		Forward.Normalize();

		// 计算点积，判断目标是否在怪物前方（点积 > 0 表示在前方）
		float DotProduct = FVector::DotProduct(Forward, ToTarget);
		
		// 只接受在怪物前方120度范围内的目标（cos(60°) ≈ 0.5）
		if (DotProduct > 0.5f)
		{
			// 再次检查距离，确保在攻击范围内
			float DistanceToTarget = FVector::Dist(EnemyLocation, Actor->GetActorLocation());
			if (DistanceToTarget <= ActualRange)
			{
				Result.Add(Actor);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("GetAttackTargetsInRangeWithTags: Found %d targets in range %.2f"), Result.Num(), ActualRange);
	return Result;
}

// ========== AI系统实现 ==========

void ABaseEnemy::SetAIState(EEnemyAIState NewState)
{
	if (CurrentAIState == NewState)
	{
		return; // 状态未改变
	}

	EEnemyAIState OldState = CurrentAIState;
	CurrentAIState = NewState;

	UE_LOG(LogTemp, Log, TEXT("Enemy %s AI State changed: %d -> %d"), *GetName(), (int32)OldState, (int32)NewState);

	// 状态切换时的处理
	switch (NewState)
	{
	case EEnemyAIState::Idle:
		StopMovement();
		break;
	case EEnemyAIState::Chase:
		// 开始追击时，确保玩家引用有效
		if (!PlayerCharacter)
		{
			PlayerCharacter = GetPlayerCharacter();
		}
		break;
	case EEnemyAIState::Attack:
		StopMovement(); // 攻击时停止移动
		// 如果不在攻击动画中，且可以攻击，则开始攻击
		if (!bIsAttacking && CanAttack())
		{
			bool bAttackStarted = StartAttack();
			if (!bAttackStarted)
			{
				// 如果攻击启动失败，切换回Chase状态，避免僵直
				UE_LOG(LogTemp, Warning, TEXT("Failed to start attack, switching back to Chase"));
				SetAIState(EEnemyAIState::Chase);
			}
		}
		break;
	case EEnemyAIState::Dead:
		StopMovement();
		break;
	default:
		break;
	}
}

bool ABaseEnemy::IsPlayerInAttackRange() const
{
	if (!PlayerCharacter || !EnemyConfig)
	{
		return false;
	}

	float Distance = GetDistanceToPlayer();
	float AttackRange = EnemyConfig->AttackRange;

	return Distance <= AttackRange;
}

bool ABaseEnemy::IsPlayerDetected() const
{
	// 这是一个const方法，只检查不修改状态
	// 检查玩家引用是否存在且在检测范围内
	if (!PlayerCharacter || !IsValid(PlayerCharacter))
	{
		return false;
	}

	// 检查玩家是否在检测范围内
	if (!DetectionSphere)
	{
		return false;
	}

	// 计算距离
	float Distance = GetDistanceToPlayer();
	float DetectionRange = EnemyConfig ? EnemyConfig->DetectionRange : 1000.0f;

	// 检查玩家是否在检测范围内
	return Distance <= DetectionRange;
}

ACharacter* ABaseEnemy::GetPlayerCharacter() const
{
	// 使用UGameplayStatics获取玩家角色
	if (UWorld* World = GetWorld())
	{
		return Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}
	return nullptr;
}

void ABaseEnemy::UpdateAI(float DeltaTime)
{
	// 如果处于受击硬直状态，切换到Hit状态
	if (CurrentHitState == EEnemyHitState::Hit)
	{
		if (CurrentAIState != EEnemyAIState::Hit)
		{
			SetAIState(EEnemyAIState::Hit);
		}
		return; // 受击时暂停AI更新
	}

	// 如果受击状态结束，恢复AI状态
	if (CurrentAIState == EEnemyAIState::Hit && CurrentHitState == EEnemyHitState::Normal)
	{
		// 重新决定状态
		EEnemyAIState NextState = DetermineNextState();
		SetAIState(NextState);
	}

	// 如果已死亡，切换到Dead状态
	if (bIsDead)
	{
		SetAIState(EEnemyAIState::Dead);
		return;
	}

	// 更新玩家引用（玩家可能重新生成）
	if (!PlayerCharacter || !IsValid(PlayerCharacter))
	{
		PlayerCharacter = GetPlayerCharacter();
	}

	// 根据当前状态执行相应行为
	switch (CurrentAIState)
	{
	case EEnemyAIState::Idle:
		// 待机状态：检测玩家
		if (DetectPlayer())
		{
			SetAIState(EEnemyAIState::Chase);
		}
		break;

	case EEnemyAIState::Chase:
		// 追击状态：移动到玩家位置
		if (!PlayerCharacter || !IsValid(PlayerCharacter))
		{
			// 玩家不存在，回到待机
			SetAIState(EEnemyAIState::Idle);
		}
		// 检查是否应该闪避（在追击过程中）
		else if (ShouldDodge())
		{
			PerformDodge();
			return;  // 执行闪避，不继续追击
		}
		else if (IsPlayerInAttackRange() && CanAttack())
		{
			// 进入攻击范围，且可以攻击（攻击动画已设置），切换到攻击状态
			SetAIState(EEnemyAIState::Attack);
		}
		else if (!DetectPlayer())
		{
			// 玩家离开检测范围，回到待机
			SetAIState(EEnemyAIState::Idle);
		}
		else
		{
			// 继续追击（即使玩家在攻击范围内但无法攻击，也继续追击）
			ChasePlayer(DeltaTime);
		}
		break;

	case EEnemyAIState::Attack:
		// 攻击状态：检查是否还在攻击范围内
		if (!PlayerCharacter || !IsValid(PlayerCharacter))
		{
			SetAIState(EEnemyAIState::Idle);
		}
		// 如果正在攻击动画中，等待攻击完成，不要切换状态
		else if (bIsAttacking)
		{
			// 攻击动画播放中，保持Attack状态，不切换
			// 这样可以避免攻击动画被打断
		}
		else if (!IsPlayerInAttackRange())
		{
			// 攻击动画已结束，玩家离开攻击范围，继续追击
			SetAIState(EEnemyAIState::Chase);
		}
		else if (!CanAttack())
		{
			// 如果无法攻击（攻击动画未设置或冷却中），继续追击而不是僵直
			SetAIState(EEnemyAIState::Chase);
		}
		else
		{
			// 如果不在攻击动画中，且可以攻击，则开始攻击
			if (!bIsAttacking && CanAttack())
			{
				StartAttack();
			}
		}
		break;

	case EEnemyAIState::Dodge:
		// 闪避状态：等待闪避动画完成
		// 闪避逻辑由PerformDodge和OnDodgeMontageEnded管理
		// 这里只需要确保在闪避期间不执行其他行为
		if (!bIsDodging)
		{
			// 如果闪避状态已结束但AI状态还没切换，强制切换
			// 这通常不应该发生，但作为安全措施
			if (IsPlayerInAttackRange())
			{
				SetAIState(EEnemyAIState::Attack);
			}
			else if (IsPlayerDetected())
			{
				SetAIState(EEnemyAIState::Chase);
			}
			else
			{
				SetAIState(EEnemyAIState::Idle);
			}
		}
		break;

	case EEnemyAIState::Hit:
		// 受击状态：已在上面处理
		break;

	case EEnemyAIState::Dead:
		// 死亡状态：不执行任何操作
		break;
	}
}

bool ABaseEnemy::DetectPlayer()
{
	if (!DetectionSphere || !GetWorld())
	{
		return false;
	}

	// 获取检测范围内的所有Actor
	TArray<AActor*> OverlappingActors;
	DetectionSphere->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	// 查找具有"Player"标签的角色
	for (AActor* Actor : OverlappingActors)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			if (Character->ActorHasTag(FName("Player")))
			{
				// 更新玩家引用
				PlayerCharacter = Character;
				return true;
			}
		}
	}

	return false;
}

float ABaseEnemy::GetDistanceToPlayer() const
{
	if (!PlayerCharacter || !IsValid(PlayerCharacter))
	{
		return FLT_MAX; // 返回最大距离，表示玩家不在范围内
	}

	FVector EnemyLocation = GetActorLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	
	return FVector::Dist(EnemyLocation, PlayerLocation);
}

void ABaseEnemy::ChasePlayer(float DeltaTime)
{
	if (!PlayerCharacter || !IsValid(PlayerCharacter) || !GetCharacterMovement())
	{
		return;
	}

	// 如果处于受击硬直状态，不执行追击
	if (CurrentHitState == EEnemyHitState::Hit)
	{
		return;
	}

	// 获取玩家位置
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector EnemyLocation = GetActorLocation();

	// 计算方向向量（忽略Z轴高度差）
	FVector Direction = PlayerLocation - EnemyLocation;
	Direction.Z = 0.0f; // 只在水平面移动
	
	// 计算距离
	float Distance = Direction.Size();
	float AttackRange = EnemyConfig ? EnemyConfig->AttackRange : 150.0f;

	// 如果距离大于攻击范围，继续移动
	if (Distance > AttackRange)
	{
		// 归一化方向向量
		Direction.Normalize();

		// 直接朝向玩家（正面朝向，而不是朝向移动方向）
		FRotator TargetRotation = Direction.Rotation();
		// 使用插值平滑旋转，插值速度为10.0（可以根据需要调整）
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
		SetActorRotation(NewRotation);

		// 使用AddMovementInput移动（朝向已经设置好了，直接向前移动）
		AddMovementInput(GetActorForwardVector(), 1.0f);

		// 更新移动速度（从配置中读取）
		if (EnemyConfig)
		{
			GetCharacterMovement()->MaxWalkSpeed = EnemyConfig->MoveSpeed;
		}
	}
	else
	{
		// 已在攻击范围内，但仍然朝向玩家（准备攻击）
		Direction.Normalize();
		FRotator TargetRotation = Direction.Rotation();
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
		SetActorRotation(NewRotation);

		// 如果已经在攻击范围内，且可以攻击，立即切换状态（不等UpdateAI）
		// 这样可以避免状态切换延迟导致的僵直
		if (CanAttack() && CurrentAIState == EEnemyAIState::Chase)
		{
			SetAIState(EEnemyAIState::Attack);
		}
	}
}

void ABaseEnemy::StopMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
}

EEnemyAIState ABaseEnemy::DetermineNextState() const
{
	// 如果已死亡，返回Dead
	if (bIsDead)
	{
		return EEnemyAIState::Dead;
	}

	// 如果处于受击硬直状态，返回Hit
	if (CurrentHitState == EEnemyHitState::Hit)
	{
		return EEnemyAIState::Hit;
	}

	// 检查玩家是否存在且在检测范围内
	if (PlayerCharacter && IsValid(PlayerCharacter))
	{
		// 如果玩家在攻击范围内，且可以攻击，返回Attack
		if (IsPlayerInAttackRange() && CanAttack())
		{
			return EEnemyAIState::Attack;
		}
		// 否则返回Chase
		return EEnemyAIState::Chase;
	}

	// 默认返回Idle
	return EEnemyAIState::Idle;
}

// ========== 攻击系统实现 ==========

void ABaseEnemy::PerformAttack()
{
	// 如果已经死亡或正在受击硬直，不执行攻击
	if (bIsDead || CurrentHitState == EEnemyHitState::Hit)
	{
		return;
	}

	// 防止同一攻击动画中重复判定
	if (bIsAttacking && AlreadyHitTargetsInThisAttack.Num() > 0)
	{
		// 如果已经判定过，不再重复判定（除非是新的攻击动画）
		return;
	}

	// 获取事件中心
	UEventCenter* EventCenter = GetEventCenter();
	if (!EventCenter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PerformAttack: EventCenter is null!"));
		return;
	}

	// 获取攻击范围内的目标
	TArray<AActor*> Targets = GetAttackTargetsInRange();
	if (Targets.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PerformAttack: No targets in range"));
		return;
	}

	// 获取攻击力
	int32 AttackDamage = EnemyConfig ? EnemyConfig->Attack : 10;

	// 对每个目标造成伤害
	for (AActor* Target : Targets)
	{
		// 检查是否已经命中过（防止同一攻击动画中重复判定）
		if (AlreadyHitTargetsInThisAttack.Contains(Target))
		{
			continue;
		}

		// 标记为已命中
		AlreadyHitTargetsInThisAttack.Add(Target);

		// 通过事件中心造成伤害
		float ActualDamage = EventCenter->MakeDamage(
			Target,                    // 被伤害对象
			static_cast<float>(AttackDamage), // 伤害数值
			GetController(),           // EventInstigator
			this                       // DamageCauser
		);

		UE_LOG(LogTemp, Log, TEXT("Enemy %s attacked %s, dealt %f damage"), 
			*GetName(), *Target->GetName(), ActualDamage);
	}
}

bool ABaseEnemy::StartAttack()
{
	// 检查是否可以攻击
	if (!CanAttack())
	{
		return false;
	}

	// 检查是否有攻击动画
	if (!AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartAttack: AttackMontage is null!"));
		return false;
	}

	// 获取动画实例
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartAttack: AnimInstance is null!"));
		return false;
	}

	// 如果已经在播放攻击动画，不重复播放
	if (AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		return false;
	}

	// 清除本次攻击的已命中目标列表
	AlreadyHitTargetsInThisAttack.Empty();

	// 设置攻击标志
	bIsAttacking = true;

	// 播放攻击动画
	// Montage_Play会默认使用蒙太奇的插槽，通常会覆盖状态机输出
	float PlayRate = 1.0f;
	float PlayTime = AnimInstance->Montage_Play(AttackMontage, PlayRate);
	
	if (PlayTime <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy %s failed to play attack montage!"), *GetName());
		bIsAttacking = false; // 播放失败，重置标志
		return false;
	}

	// 绑定动画完成回调
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &ABaseEnemy::OnAttackMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);

	UE_LOG(LogTemp, Log, TEXT("Enemy %s started attack (PlayTime: %.2f)"), *GetName(), PlayTime);

	return true;
}

bool ABaseEnemy::CanAttack() const
{
	// 如果已死亡或处于受击硬直状态，不能攻击
	if (bIsDead || CurrentHitState == EEnemyHitState::Hit)
	{
		return false;
	}

	// 检查攻击冷却是否结束
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(AttackCooldownTimer))
	{
		return false;
	}

	// 检查是否有攻击动画
	if (!AttackMontage)
	{
		return false;
	}

	return true;
}

void ABaseEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 重置攻击标志
	bIsAttacking = false;

	// 清除已命中目标列表
	AlreadyHitTargetsInThisAttack.Empty();

	// 计算攻击冷却时间
	float CooldownTime = 1.0f;
	if (EnemyConfig && EnemyConfig->AttackSpeed > 0.0f)
	{
		// 冷却时间 = 1.0 / 攻击速度
		CooldownTime = 1.0f / EnemyConfig->AttackSpeed;
	}

	// 设置攻击冷却计时器
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			AttackCooldownTimer,
			CooldownTime,
			false
		);
		AttackCooldownRemaining = CooldownTime;
	}

	UE_LOG(LogTemp, Log, TEXT("Enemy %s attack ended, cooldown: %.2f seconds"), 
		*GetName(), CooldownTime);

	// 如果被中断（例如受击），不切换状态
	if (bInterrupted)
	{
		return;
	}

	// 攻击动画结束后，根据当前情况决定下一个状态
	// 这个逻辑会在UpdateAI中处理，这里不需要手动切换
}

// ========== 闪避系统实现 ==========

bool ABaseEnemy::CanDodge() const
{
	// 已经死亡、正在闪避、正在受击硬直、冷却中，不能闪避
	if (bIsDead || bIsDodging || CurrentHitState == EEnemyHitState::Hit)
	{
		return false;
	}
	
	// 检查冷却时间
	if (DodgeCooldownTimerHandle.IsValid())
	{
		return false;  // 冷却中
	}
	
	// 检查是否在地面上
	if (GetCharacterMovement() && !GetCharacterMovement()->IsMovingOnGround())
	{
		return false;
	}
	
	return true;
}

bool ABaseEnemy::IsPlayerAttacking() const
{
	if (!PlayerCharacter)
	{
		return false;
	}
	
	// 通过角色类的IsAttacking方法检测
	if (Ablack_moneyCharacter* Player = Cast<Ablack_moneyCharacter>(PlayerCharacter))
	{
		return Player->IsAttacking();
	}
	
	return false;
}

bool ABaseEnemy::ShouldDodge() const
{
	if (!CanDodge() || !PlayerCharacter)
	{
		return false;
	}
	
	// 检查玩家是否在攻击
	if (!IsPlayerAttacking())
	{
		return false;
	}
	
	// 检查距离
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());
	if (DistanceToPlayer > DodgeRange)
	{
		return false;  // 距离太远，不需要闪避
	}
	
	// 基于概率判断
	float RandomValue = FMath::FRand();
	return RandomValue <= DodgeProbability;
}

void ABaseEnemy::PerformDodge()
{
	if (!CanDodge())
	{
		return;
	}
	
	// 检查闪避蒙太奇是否设置
	if (!DodgeMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("PerformDodge: DodgeMontage is null!"));
		return;
	}
	
	// 获取动画实例
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("PerformDodge: AnimInstance is null!"));
		return;
	}
	
	// 如果已经在播放闪避动画，不重复播放
	if (AnimInstance->Montage_IsPlaying(DodgeMontage))
	{
		return;
	}
	
	// 停止其他动画（如攻击动画）
	if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Stop(0.1f, AttackMontage);
	}
	
	// 设置闪避状态
	bIsDodging = true;
	bInvulnerableDuringDodge = true;
	
	// 计算闪避方向（远离玩家）
	FVector DodgeDirection = FVector::ZeroVector;
	if (PlayerCharacter)
	{
		FVector ToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
		ToPlayer.Z = 0.0f;
		ToPlayer.Normalize();
		DodgeDirection = -ToPlayer;  // 远离玩家
	}
	
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = -GetActorForwardVector();
	}
	
	DodgeDirection.Normalize();
	
	// 停止当前移动
	StopMovement();
	
	// 执行闪避移动
	LaunchCharacter(DodgeDirection * DodgeStrength, true, false);
	
	// 播放闪避动画蒙太奇
	float PlayRate = 1.0f;
	float PlayTime = AnimInstance->Montage_Play(DodgeMontage, PlayRate);
	
	if (PlayTime <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy %s failed to play dodge montage!"), *GetName());
		bIsDodging = false;
		bInvulnerableDuringDodge = false;
		return;
	}
	
	// 绑定蒙太奇结束回调
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &ABaseEnemy::OnDodgeMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DodgeMontage);
	
	// 设置AI状态为Dodge
	SetAIState(EEnemyAIState::Dodge);
	
	// 设置闪避持续时间计时器（作为备用，防止蒙太奇回调失败）
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			DodgeTimerHandle,
			this,
			&ABaseEnemy::EndDodge,
			DodgeDuration,
			false
		);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Enemy %s started dodge (PlayTime: %.2f)"), *GetName(), PlayTime);
}

void ABaseEnemy::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 清除闪避标志
	bIsDodging = false;
	bInvulnerableDuringDodge = false;
	
	// 清除计时器
	DodgeTimerHandle.Invalidate();
	
	// 设置闪避冷却
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			DodgeCooldownTimerHandle,
			this,
			&ABaseEnemy::OnDodgeCooldownEnd,
			DodgeCooldown,
			false
		);
	}
	
	// 闪避结束后，根据情况切换AI状态
	if (bIsDead)
	{
		SetAIState(EEnemyAIState::Dead);
	}
	else if (IsPlayerInAttackRange())
	{
		SetAIState(EEnemyAIState::Attack);
	}
	else if (IsPlayerDetected())
	{
		SetAIState(EEnemyAIState::Chase);
	}
	else
	{
		SetAIState(EEnemyAIState::Idle);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Enemy %s dodge ended (Interrupted: %d)"), *GetName(), bInterrupted);
}

void ABaseEnemy::EndDodge()
{
	// 如果蒙太奇已经结束，这个方法可能不会被调用
	// 但作为安全措施保留
	if (bIsDodging)
	{
		bIsDodging = false;
		bInvulnerableDuringDodge = false;
		
		// 强制停止闪避蒙太奇
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (DodgeMontage && AnimInstance->Montage_IsPlaying(DodgeMontage))
			{
				AnimInstance->Montage_Stop(0.0f, DodgeMontage);
			}
		}
		
		OnDodgeMontageEnded(nullptr, false);
	}
}

void ABaseEnemy::OnDodgeCooldownEnd()
{
	DodgeCooldownTimerHandle.Invalidate();
}

void ABaseEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 检查是否是玩家角色（Pawn）
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(Ablack_moneyCharacter::StaticClass()))
		{
			InteractionWidget->SetVisibility(true);
		}
	}
}

void ABaseEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (Cast<Ablack_moneyCharacter>(OtherActor))
		{
			InteractionWidget->SetVisibility(false);

		}
	}
}