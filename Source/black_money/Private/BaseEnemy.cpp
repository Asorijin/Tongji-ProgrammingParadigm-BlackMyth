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
	GetCharacterMovement()->bOrientRotationToMovement = false; //   ?false   ?    ?   
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1080.0f, 0.0f); //    ?   ??   500  ?1080  / ?
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	//           ¦¶      
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);
	AttackRangeSphere->SetSphereRadius(150.0f);
	AttackRangeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeSphere->SetHiddenInGame(true);

	//       ?¦¶   
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(1000.0f);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DetectionSphere->SetHiddenInGame(true);

	//   ?  ?¦Ë  ?  
	InitializePlaceholderMesh();

	//            ?   UObject?  NewObject    BeginPlay §Ô ?    
	EnemyConfig = nullptr; //   BeginPlay §Õ   

	//     ? ? ?
	Tags.Add(FName("Enemy"));

	//   ?      ??  ?   
	AttackMontage = nullptr;
	bIsAttacking = false;
	AttackCooldownRemaining = 0.0f;

	//   ?   ?             ?  
	HitMontage = nullptr;
	DeathMontage = nullptr;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	//         ?         ?   
	if (!EnemyConfig)
	{
		EnemyConfig = NewObject<UEnemyConfig>(this);
		if (EnemyConfig)
		{
			EnemyConfig->Initialize();
		}
	}

	//   ?  AI??
	CurrentAIState = EEnemyAIState::Idle;
	AIUpdateTimer = 0.0f;

	//    ? ?  ? ?    
	PlayerCharacter = GetPlayerCharacter();
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//                  AI   ? 
	if (bIsDead || CurrentAIState == EEnemyAIState::Dead)
	{
		return;
	}

	//        Chase??  ??  ?   ?   ?   ?       
	// ? ?Attack??  ?         ?     ?SetAIState(Attack)?     StopMovement
	if (CurrentAIState == EEnemyAIState::Chase && !bIsDead)
	{
		ChasePlayer(DeltaTime);
	}
	
	//        Attack??  ?  ?? ?     ?ChasePlayer  ?? §Ý ?  ?  ?    ?
	if (CurrentAIState == EEnemyAIState::Attack && !bIsDead)
	{
		//     ?? ¡ê     ?? ?   ?        ?      ?       ? 
		if (GetCharacterMovement() && GetCharacterMovement()->Velocity.Size() > 0.1f)
		{
			StopMovement();
		}
		
		//     ?? ¡ê   ?      ?      ?   
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

	//  ? AI   ? ?  
	AIUpdateTimer += DeltaTime;

	//         AI   ?    ? ??   ?   ???? §µ 
	if (AIUpdateTimer >= AIUpdateInterval)
	{
		UpdateAI(DeltaTime);
		AIUpdateTimer = 0.0f;
	}
}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	//   float?  ?int32         ? ReceiveDamage    
	ReceiveDamage(static_cast<int32>(DamageAmount), DamageCauser);
	return DamageAmount;
}

void ABaseEnemy::ReceiveDamage(int32 DamageAmount, AActor* DamageCauser)
{
	//               ? ??         ? 
	if (bIsDead || !EnemyConfig || CurrentHitState == EEnemyHitState::Invulnerable)
	{
		return;
	}

	//     ?   ?      ?       
	int32 ActualDamage = FMath::Max(1, DamageAmount - EnemyConfig->Defence);
	
	//         ?
	const int32 OldHp = EnemyConfig->CurrentHp;
	EnemyConfig->CurrentHp = FMath::Max(0, EnemyConfig->CurrentHp - ActualDamage);

	UE_LOG(LogTemp, Log, TEXT("Enemy %s took %d damage (from %d), remaining HP: %d"), 
		*GetName(), ActualDamage, OldHp, EnemyConfig->CurrentHp);

	// ?? ?         ? £       ?  
	//     ?   ?    ??    ? £ ?   UI??  

	//      ? ????
	EnterHitStun();

	//     ?     
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

	//  §Ý       AI??
	SetAIState(EEnemyAIState::Dead);

	// ?   ?     ??     ? 
	if (UEventCenter* EventCenter = GetEventCenter())
	{
		//     ?   ?    ??     ? 
		//    »Ç??UI   ?       ?        §¹  
	}

	//       §Þ ?  
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HitStunTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(InvulnerableTimerHandle);
	}

	//       ?
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// ?? ? 
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	// ??       ?  ??     ?  
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// ??        
		if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			AnimInstance->Montage_Stop(0.0f, AttackMontage);
		}
		
		// ?? ?     
		if (HitMontage && AnimInstance->Montage_IsPlaying(HitMontage))
		{
			AnimInstance->Montage_Stop(0.0f, HitMontage);
		}
		
		//             
		if (DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Enemy %s playing death animation"), *GetName());
		}
	}

	// TODO:  ?    ???     §¹            ??    Destroy()  
}

bool ABaseEnemy::IsDead() const
{
	return bIsDead || (EnemyConfig && EnemyConfig->IsDead());
}

void ABaseEnemy::InitializePlaceholderMesh()
{
	// ?  Mannequin  ??¦Ë  ?  
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
		return; //  ?   ????
	}

	CurrentHitState = EEnemyHitState::Hit;

	//  §Ý    ? AI??
	SetAIState(EEnemyAIState::Hit);

	// ?? ? 
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	//     ??  ?  
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

	//      ?     
	if (HitMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// ??  ?   ??               ?     
			if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				AnimInstance->Montage_Stop(0.2f, AttackMontage);
			}
			
			//      ?     
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

	//         ? ??
	EnterInvulnerable();

	UE_LOG(LogTemp, Log, TEXT("Enemy %s ended hit stun"), *GetName());
}

void ABaseEnemy::EnterInvulnerable()
{
	CurrentHitState = EEnemyHitState::Invulnerable;

	//      ?§Þ ?  
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
	// ? ?   "Player"  ?
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

	//    ?  ?    ¦¶  ?  AttackRangeSphere ??
	float ActualRange = AttackRange;
	if (ActualRange <= 0.0f && AttackRangeSphere)
	{
		ActualRange = AttackRangeSphere->GetScaledSphereRadius();
	}
	
	//        ?    §¹  ¦¶  ?       §Ö?     ¦¶
	if (ActualRange <= 0.0f && EnemyConfig)
	{
		ActualRange = EnemyConfig->AttackRange;
	}

	//        ? §µ ?  ?  ?
	if (ActualRange <= 0.0f)
	{
		ActualRange = 150.0f; // ? ?     ¦¶
	}

	// ? ¨´   ?    ¦Ë    ?      ?      ?      ? 
	//                 ? ?       
	FVector EnemyLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector SphereCenter = EnemyLocation + ForwardVector * (ActualRange * 0.5f); //         ?   ?  ? ?     ?

	//    ¨¹  ?      ?  ¦Ï Character   ? ? 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); //   ?   Pawn   ?   ??  ?

	// ?     ¦Ç ¦¶   
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(
		World,
		SphereCenter,
		ActualRange * 0.5f, // ?  ? ?¦¶    ?        ? ?    
		ObjectTypes,
		AActor::StaticClass(),  //         Actor
		TArray<AActor*>(),      //      §Ò   ?        ¦Ê¦Æ   
		OverlappingActors
	);

	// ??      ?    ?  Actor        ?  ?     ¦¶ ?      ? ï‚
	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || Actor == this) //  ?     
		{
			continue;
		}

		//     ?    ?   ?
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

		//      ? ï“?  ?   ?   ?    180     ¦Ç ¦¶ ? 
		FVector ToTarget = Actor->GetActorLocation() - EnemyLocation;
		ToTarget.Z = 0.0f; //    ??? 
		ToTarget.Normalize();
		
		FVector Forward = ForwardVector;
		Forward.Z = 0.0f;
		Forward.Normalize();

		//          §Ø ?   ?  ?   ?        > 0   ?  ?    
		float DotProduct = FVector::DotProduct(Forward, ToTarget);
		
		// ?     ?   ?  120 ? ¦¶ ? ? ?cos(60  )    0.5  
		if (DotProduct > 0.5f)
		{
			//  ?¦Ì    ??   ?     ¦¶  
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

// ========== AI???   ==========

void ABaseEnemy::SetAIState(EEnemyAIState NewState)
{
	if (CurrentAIState == NewState)
	{
		return; // ??¦Ä ? 
	}

	EEnemyAIState OldState = CurrentAIState;
	CurrentAIState = NewState;

	UE_LOG(LogTemp, Log, TEXT("Enemy %s AI State changed: %d -> %d"), *GetName(), (int32)OldState, (int32)NewState);

	// ?? §Ý ? ?   
	switch (NewState)
	{
	case EEnemyAIState::Idle:
		StopMovement();
		break;
	case EEnemyAIState::Chase:
		//   ??  ?  ?           §¹
		if (!PlayerCharacter)
		{
			PlayerCharacter = GetPlayerCharacter();
		}
		break;
	case EEnemyAIState::Attack:
		StopMovement(); //     ??? ? 
		//       ?        §µ  ?  ?       ?    
		if (!bIsAttacking && CanAttack())
		{
			bool bAttackStarted = StartAttack();
			if (!bAttackStarted)
			{
				//           ? ?  §Ý   Chase??     ??
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
	//     ?  const      ?  öö ? ??
	//            ?       ? ?¦¶  
	if (!PlayerCharacter || !IsValid(PlayerCharacter))
	{
		return false;
	}

	//        ?  ? ?¦¶  
	if (!DetectionSphere)
	{
		return false;
	}

	//        
	float Distance = GetDistanceToPlayer();
	float DetectionRange = EnemyConfig ? EnemyConfig->DetectionRange : 1000.0f;

	//        ?  ? ?¦¶  
	return Distance <= DetectionRange;
}

ACharacter* ABaseEnemy::GetPlayerCharacter() const
{
	// ?  UGameplayStatics  ?  ? ?
	if (UWorld* World = GetWorld())
	{
		return Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}
	return nullptr;
}

void ABaseEnemy::UpdateAI(float DeltaTime)
{
	//         ? ????   §Ý   Hit??
	if (CurrentHitState == EEnemyHitState::Hit)
	{
		if (CurrentAIState != EEnemyAIState::Hit)
		{
			SetAIState(EEnemyAIState::Hit);
		}
		return; //  ? ?  ?AI    
	}

	//     ? ??       ? AI??
	if (CurrentAIState == EEnemyAIState::Hit && CurrentHitState == EEnemyHitState::Normal)
	{
		//    ?   ??
		EEnemyAIState NextState = DetermineNextState();
		SetAIState(NextState);
	}

	//             §Ý   Dead??
	if (bIsDead)
	{
		SetAIState(EEnemyAIState::Dead);
		return;
	}

	//           ?   ?          ? 
	if (!PlayerCharacter || !IsValid(PlayerCharacter))
	{
		PlayerCharacter = GetPlayerCharacter();
	}

	//    ? ????    ?  ?
	switch (CurrentAIState)
	{
	case EEnemyAIState::Idle:
		//     ??        
		if (DetectPlayer())
		{
			SetAIState(EEnemyAIState::Chase);
		}
		break;

	case EEnemyAIState::Chase:
		// ?  ??   ?      ¦Ë  
		if (!PlayerCharacter || !IsValid(PlayerCharacter))
		{
			//   ?    ?  ?     
			SetAIState(EEnemyAIState::Idle);
		}
		else if (IsPlayerInAttackRange() && CanAttack())
		{
			//    ?    ¦¶   ?  ?                  ?    §Ý       ??
			SetAIState(EEnemyAIState::Attack);
		}
		else if (!DetectPlayer())
		{
			//     ?  ?¦¶   ?     
			SetAIState(EEnemyAIState::Idle);
		}
		else
		{
			//     ?      ?    ?     ¦¶ ?  ?       ?    ?    
			ChasePlayer(DeltaTime);
		}
		break;

	case EEnemyAIState::Attack:
		//     ??      ?  ?     ¦¶  
		if (!PlayerCharacter || !IsValid(PlayerCharacter))
		{
			SetAIState(EEnemyAIState::Idle);
		}
		//       ?        §µ  ?       ?   ? §Ý ??
		else if (bIsAttacking)
		{
			//              §µ     Attack??     §Ý 
			//        ?  ?           
		}
		else if (!IsPlayerInAttackRange())
		{
			//          ?         ?      ¦¶      ?  
			SetAIState(EEnemyAIState::Chase);
		}
		else if (!CanAttack())
		{
			//     ?               ¦Ä   ?   ? §µ       ?       ? ?
			SetAIState(EEnemyAIState::Chase);
		}
		else
		{
			//       ?        §µ  ?  ?       ?    
			if (!bIsAttacking && CanAttack())
			{
				StartAttack();
			}
		}
		break;

	case EEnemyAIState::Dodge:
		// TODO:  ??   ?       ? 
		break;

	case EEnemyAIState::Hit:
		//  ? ??         ›¨  
		break;

	case EEnemyAIState::Dead:
		//     ??    ?   ¦Ê¦Â   
		break;
	}
}

bool ABaseEnemy::DetectPlayer()
{
	if (!DetectionSphere || !GetWorld())
	{
		return false;
	}

	//   ?  ?¦¶ ?     Actor
	TArray<AActor*> OverlappingActors;
	DetectionSphere->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	//    ?   "Player"  ? ? ?
	for (AActor* Actor : OverlappingActors)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			if (Character->ActorHasTag(FName("Player")))
			{
				//            
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
		return FLT_MAX; //          ?  ?  ?  ? ¦¶  
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

	//         ? ????    ?  ?  
	if (CurrentHitState == EEnemyHitState::Hit)
	{
		return;
	}

	//   ?   ¦Ë  
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector EnemyLocation = GetActorLocation();

	//    ?            Z  ???
	FVector Direction = PlayerLocation - EnemyLocation;
	Direction.Z = 0.0f; // ?  ??   ? 
	
	//        
	float Distance = Direction.Size();
	float AttackRange = EnemyConfig ? EnemyConfig->AttackRange : 150.0f;

	//          ?     ¦¶       ? 
	if (Distance > AttackRange)
	{
		//   ?          
		Direction.Normalize();

		// ? ?     ?    ãý ??    ?    ?     
		FRotator TargetRotation = Direction.Rotation();
		// ? ¨° ??    ?    ? ? ?10.0     ?     ?      
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
		SetActorRotation(NewRotation);

		// ?  AddMovementInput ?        ?    ¨²  ? ?    ? ?   
		AddMovementInput(GetActorForwardVector(), 1.0f);

		//      ?  ??        §Ø ?  
		if (EnemyConfig)
		{
			GetCharacterMovement()->MaxWalkSpeed = EnemyConfig->MoveSpeed;
		}
	}
	else
	{
		//    ?     ¦¶ ?     ?      ? ?        
		Direction.Normalize();
		FRotator TargetRotation = Direction.Rotation();
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
		SetActorRotation(NewRotation);

		//     ?  ?     ¦¶ ?  ?  ?          §Ý ??      UpdateAI  
		//        ?   ?? §Ý  ??  ¦Ì? ?
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
	//                Dead
	if (bIsDead)
	{
		return EEnemyAIState::Dead;
	}

	//         ? ????      Hit
	if (CurrentHitState == EEnemyHitState::Hit)
	{
		return EEnemyAIState::Hit;
	}

	//        ?       ? ?¦¶  
	if (PlayerCharacter && IsValid(PlayerCharacter))
	{
		//        ?     ¦¶ ?  ?  ?         Attack
		if (IsPlayerInAttackRange() && CanAttack())
		{
			return EEnemyAIState::Attack;
		}
		//    ?? Chase
		return EEnemyAIState::Chase;
	}

	// ? ?   Idle
	return EEnemyAIState::Idle;
}

// ==========     ???   ==========

void ABaseEnemy::PerformAttack()
{
	//     ?            ? ??    ? §Û   
	if (bIsDead || CurrentHitState == EEnemyHitState::Hit)
	{
		return;
	}

	//   ???           ?  §Ø 
	if (bIsAttacking && AlreadyHitTargetsInThisAttack.Num() > 0)
	{
		//     ?  §Ø          ?  §Ø          ¦Ì?         
		return;
	}

	//   ? ?     
	UEventCenter* EventCenter = GetEventCenter();
	if (!EventCenter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PerformAttack: EventCenter is null!"));
		return;
	}

	//   ?      ¦¶ ? ?  
	TArray<AActor*> Targets = GetAttackTargetsInRange();
	if (Targets.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PerformAttack: No targets in range"));
		return;
	}

	//   ?      
	int32 AttackDamage = EnemyConfig ? EnemyConfig->Attack : 10;

	//   ?  ?      ? 
	for (AActor* Target : Targets)
	{
		//     ?  ?    §Û     ???           ?  §Ø   
		if (AlreadyHitTargetsInThisAttack.Contains(Target))
		{
			continue;
		}

		//    ?      
		AlreadyHitTargetsInThisAttack.Add(Target);

		// ?   ?         ? 
		float ActualDamage = EventCenter->MakeDamage(
			Target,                    //    ?     
			static_cast<float>(AttackDamage), //  ?   ?
			GetController(),           // EventInstigator
			this                       // DamageCauser
		);

		UE_LOG(LogTemp, Log, TEXT("Enemy %s attacked %s, dealt %f damage"), 
			*GetName(), *Target->GetName(), ActualDamage);
	}
}

bool ABaseEnemy::StartAttack()
{
	//     ?   ?   
	if (!CanAttack())
	{
		return false;
	}

	//     ?  §Û       
	if (!AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartAttack: AttackMontage is null!"));
		return false;
	}

	//   ?    ?  
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartAttack: AnimInstance is null!"));
		return false;
	}

	//     ?  ?  ?            ?     
	if (AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		return false;
	}

	//       ¦É           ?   §Ò 
	AlreadyHitTargetsInThisAttack.Empty();

	//    ¨´     ?
	bIsAttacking = true;

	//    ?       
	// Montage_Play  ?  ?    ?  ? ? ?   ?  ??     
	float PlayRate = 1.0f;
	float PlayTime = AnimInstance->Montage_Play(AttackMontage, PlayRate);
	
	if (PlayTime <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy %s failed to play attack montage!"), *GetName());
		bIsAttacking = false; //     ? ?    ? ?
		return false;
	}

	//  ??     ?? 
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &ABaseEnemy::OnAttackMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);

	UE_LOG(LogTemp, Log, TEXT("Enemy %s started attack (PlayTime: %.2f)"), *GetName(), PlayTime);

	return true;
}

bool ABaseEnemy::CanAttack() const
{
	//               ? ????     ?   
	if (bIsDead || CurrentHitState == EEnemyHitState::Hit)
	{
		return false;
	}

	//   ûp    ? ?    
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(AttackCooldownTimer))
	{
		return false;
	}

	//     ?  §Û       
	if (!AttackMontage)
	{
		return false;
	}

	return true;
}

void ABaseEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//    ¨´     ?
	bIsAttacking = false;

	//          ?   §Ò 
	AlreadyHitTargetsInThisAttack.Empty();

	//    ?    ??  
	float CooldownTime = 1.0f;
	if (EnemyConfig && EnemyConfig->AttackSpeed > 0.0f)
	{
		//   ??   = 1.0 /      ? 
		CooldownTime = 1.0f / EnemyConfig->AttackSpeed;
	}

	//    ¨´     ?  ?  
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

	//       §Ø?      ?        §Ý ??
	if (bInterrupted)
	{
		return;
	}

	//              ??  ? ?         ?  ??
	//     ?     UpdateAI §Õ       ?  ? ?  §Ý 
}

