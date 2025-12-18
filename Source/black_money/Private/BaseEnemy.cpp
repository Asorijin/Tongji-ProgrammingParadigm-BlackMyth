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

ABaseEnemy::ABaseEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Don't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
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

	// 创建检测范围组件
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(1000.0f);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DetectionSphere->SetHiddenInGame(true);

	// 初始化占位符模型
	InitializePlaceholderMesh();

	// 创建怪物配置对象（UObject使用NewObject，在BeginPlay中初始化）
	EnemyConfig = nullptr; // 在BeginPlay中创建

	// 设置默认标签
	Tags.Add(FName("Enemy"));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 创建并初始化怪物配置对象
	if (!EnemyConfig)
	{
		EnemyConfig = NewObject<UEnemyConfig>(this);
		if (EnemyConfig)
		{
			EnemyConfig->Initialize();
		}
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

	// 如果已死亡，不更新AI和移动
	if (bIsDead || CurrentAIState == EEnemyAIState::Dead)
	{
		return;
	}

	// 如果处于Chase状态，每帧都执行移动（确保移动连续）
	if (CurrentAIState == EEnemyAIState::Chase && !bIsDead)
	{
		ChasePlayer(DeltaTime);
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

	// TODO: 播放死亡动画
	// TODO: 延迟销毁或播放死亡效果
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

	FVector SphereCenter = GetActorLocation();

	// 设置检测的对象类型（参考Character类的实现）
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // 主要检测Pawn类型（玩家和怪物）

	// 执行球形范围检测
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(
		World,
		SphereCenter,
		ActualRange,
		ObjectTypes,
		AActor::StaticClass(),  // 查找所有Actor
		TArray<AActor*>(),      // 忽略列表（空，不忽略任何对象）
		OverlappingActors
	);

	// 筛选出具有指定标签的Actor
	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || Actor == this) // 排除自身
		{
			continue;
		}

		// 检查是否具有目标标签
		for (const FName& TagName : TargetTags)
		{
			if (Actor->ActorHasTag(TagName))
			{
				Result.Add(Actor);
				break; // 找到一个匹配的标签即可，避免重复添加
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
		else if (IsPlayerInAttackRange())
		{
			// 进入攻击范围，切换到攻击状态
			SetAIState(EEnemyAIState::Attack);
		}
		else if (!DetectPlayer())
		{
			// 玩家离开检测范围，回到待机
			SetAIState(EEnemyAIState::Idle);
		}
		else
		{
			// 继续追击
			ChasePlayer(DeltaTime);
		}
		break;

	case EEnemyAIState::Attack:
		// 攻击状态：检查是否还在攻击范围内
		if (!PlayerCharacter || !IsValid(PlayerCharacter))
		{
			SetAIState(EEnemyAIState::Idle);
		}
		else if (!IsPlayerInAttackRange())
		{
			// 玩家离开攻击范围，继续追击
			SetAIState(EEnemyAIState::Chase);
		}
		// TODO: 在阶段四实现攻击逻辑
		break;

	case EEnemyAIState::Dodge:
		// TODO: 在阶段五实现闪避逻辑
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

		// 停止移动
		StopMovement();
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
		// 如果玩家在攻击范围内，返回Attack
		if (IsPlayerInAttackRange())
		{
			return EEnemyAIState::Attack;
		}
		// 否则返回Chase
		return EEnemyAIState::Chase;
	}

	// 默认返回Idle
	return EEnemyAIState::Idle;
}

