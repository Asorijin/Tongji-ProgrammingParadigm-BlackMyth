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
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
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
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

