// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"

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

void ABaseEnemy::TakeDamage(int32 DamageAmount)
{
	if (bIsDead || !EnemyConfig)
	{
		return;
	}

	// 计算实际伤害（考虑防御力）
	int32 ActualDamage = FMath::Max(1, DamageAmount - EnemyConfig->Defence);
	
	// 减少生命值
	EnemyConfig->CurrentHp = FMath::Max(0, EnemyConfig->CurrentHp - ActualDamage);

	UE_LOG(LogTemp, Log, TEXT("Enemy %s took %d damage, remaining HP: %d"), 
		*GetName(), ActualDamage, EnemyConfig->CurrentHp);

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

	// TODO: 通过事件中心通知死亡事件
	// 后续阶段会实现事件中心集成

	// 禁用碰撞
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
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

