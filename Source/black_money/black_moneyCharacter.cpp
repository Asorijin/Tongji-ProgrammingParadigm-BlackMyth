#include "black_moneyCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Engine/AssetManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include"Animation/AnimMontage.h"
#include "Tools.h"
#include "LandTemple.h"
#include "black_moneyGameInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EventCenter.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// Ablack_moneyCharacter

Ablack_moneyCharacter::Ablack_moneyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false; // 禁用碰撞检测，防止怪物攻击时相机拉近

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// ��������������
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(500.0f);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	// ����������ײ��
	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	WeaponHitBox->InitBoxExtent(FVector(10.f, 30.f, 10.f));

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);    // Ĭ�Ϲر�
	WeaponHitBox->SetCollisionObjectType(ECC_Pawn);
	WeaponHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);  // �� Pawn �ص�

	WeaponHitBox->SetHiddenInGame(false);//���ڵ��Բ鿴������Ϊtrue

	// ���ÿ��ӻ�
	DetectionSphere->SetHiddenInGame(false);

	//�����������
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;

}

//////////////////////////////////////////////////////////////////////////
// Input

void Ablack_moneyCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void Ablack_moneyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Ablack_moneyCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Ablack_moneyCharacter::Look);
		// �� Ctrl -> Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::Dodge);

		//�󶨹�������
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::Attack);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void Ablack_moneyCharacter::Move(const FInputActionValue& Value)
{
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void Ablack_moneyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
// ���ܹ���
void Ablack_moneyCharacter::Dodge() { // ����Ƿ�������� 
	if (!CanDodge()) { return; }
	// �������ܷ�������ʹ������ƶ����뷽�����ޣ���ʹ�ý�ɫǰ��
	FVector DodgeDirection = GetLastMovementInputVector().GetSafeNormal();
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = GetActorForwardVector();
	}
	// ��������״̬
	bIsDodging = true;
	// ������ˮƽ����
	DodgeDirection.Z = 0.0f;
	DodgeDirection = DodgeDirection.GetSafeNormal();

	// Ӧ�ó����������ܣ����� XY �ٶȣ��������� Z��
	LaunchCharacter(DodgeDirection * DodgeStrength, /*bXYOverride*/ true, /*bZOverride*/ false);

	
	// ��ʱ�������޵�֡���������ȱ��� true/false ������ν
	bInvulnerableDuringDodge = false;

	// ����������̫��
	if (DodgeMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DodgeMontage, DodgePlayRate);
			if (GEngine)
			{
				
				GEngine->AddOnScreenDebugMessage(
					/*Key*/ -1,
					/*Time*/ 2.0f,
					FColor::Green,
					TEXT("Dodge: Play ")
				);
			}
			
		}
	}

}


bool Ablack_moneyCharacter::CanDodge() const
{
	// ������������ӵ�п��������ƶ����
	if (bIsDodging || Controller == nullptr || GetCharacterMovement() == nullptr)
	{
		return false;
	}

	// �����޶��������ܿ����ã�
	// return GetCharacterMovement()->IsMovingOnGround();
	return true;
}

void Ablack_moneyCharacter::EndDodge()
{
	bIsDodging = false;
	bInvulnerableDuringDodge = false;

}


void Ablack_moneyCharacter::Attack()
{
	// �����в��ܹ���
	if (bIsDodging)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	const bool bMontagePlaying = (AnimInstance && AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage));

	// �������������ڹ����� / ���������� / û����̫�� / ��̫��û�ڲ�
	if (!bIsAttacking || ComboIndex <= 0 || !AttackMontage || !bMontagePlaying)
	{
		ComboIndex = 1;
		bIsAttacking = true;
		bCanQueueNextCombo = false;

		if (AnimInstance && AttackMontage)
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName(TEXT("Attack1")), AttackMontage);
		}
		return;
	}

	// ���ڹ����У�ֻ�����������ڿ���ʱ����������Ч
	if (!bCanQueueNextCombo)
	{
		return;
	}

	// �����������е���һ��
	if (AnimInstance && AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		int32 NextComboIndex = ComboIndex + 1;
		if (NextComboIndex > 4)
		{
			NextComboIndex = 1;
		}
		FString DebugMsg = FString::Printf(TEXT("Jump Combo %d -> %d"), ComboIndex, NextComboIndex);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, DebugMsg);
		}
		FName NextSectionName;
		switch (NextComboIndex)
		{
		case 1: NextSectionName = FName(TEXT("Attack1")); break;
		case 2: NextSectionName = FName(TEXT("Attack2")); break;
		case 3: NextSectionName = FName(TEXT("Attack3")); break;
		case 4: NextSectionName = FName(TEXT("Attack4")); break;
		default: NextSectionName = FName(TEXT("Attack1")); break;
		}

		ComboIndex = NextComboIndex;
		bCanQueueNextCombo = false;
		AnimInstance->Montage_JumpToSection(NextSectionName, AttackMontage);
	}
}
void Ablack_moneyCharacter::EnableComboWindow()
{
	// ��ǰ�ε����������ʱ���
	bCanQueueNextCombo = true;
}

void Ablack_moneyCharacter::OnAttackSectionEnded()
{
	// ��ǰ�γ��׽���������ʱ��δ�����������׹�������
	bCanQueueNextCombo = false;

	// ������������
	ComboIndex = 0;
	bIsAttacking = false;
}
void Ablack_moneyCharacter::OnWeaponHitBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// ֻ�ڹ���״̬�ҵ�ǰ HitBox ����ʱ����Ч
	if (!bIsAttacking || !WeaponHitBox ||
		WeaponHitBox->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		return;
	}
	// �ų���Ч���������
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// һ�ι��������ڷ�ֹ�ظ�����
	if (AlreadyHitActors.Contains(OtherActor))
	{
		return;
	}
	AlreadyHitActors.Add(OtherActor);


	// ͨ�� EventCenter �����˺�
	// ͨ�� EventCenter �����˺�
	if (EventCenter && characterConfig)
	{
		const float DamageValue = static_cast<float>(characterConfig->_attack);

		EventCenter->MakeDamage(
			OtherActor,      // ���˺�����
			DamageValue,     // �˺���ֵ���������ã�
			GetController(), // Instigator
			this             // ��ɫ�Լ�
		);
	}
}

void Ablack_moneyCharacter::StartAttackHit()
{
	if (!WeaponHitBox)
	{
		return;
	}

	AlreadyHitActors.Empty();

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponHitBox->SetHiddenInGame(false);
}
void Ablack_moneyCharacter::EndAttackHit()
{
	if (!WeaponHitBox)
	{
		return;
	}

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHitBox->SetHiddenInGame(true);
	AlreadyHitActors.Empty();
}
void Ablack_moneyCharacter::BeginPlay() {

	Super::BeginPlay();

	characterConfig = NewObject<UCharacterConfig>();
	characterConfig->Initialize();

	// ��������ײ�ص�ί��
	if (WeaponHitBox)
	{
		WeaponHitBox->OnComponentBeginOverlap.AddDynamic(this,&Ablack_moneyCharacter::OnWeaponHitBoxBeginOverlap);
	}

	// �� GameInstance ��ȡ EventCenter������ TriggerNearByInteractions ����÷���
	if (Ublack_moneyGameInstance* GI = Cast<Ublack_moneyGameInstance>(GetGameInstance()))
	{
		EventCenter = GI->GetEventCenter();
	}

}

void Ablack_moneyCharacter::Tick(float deltaTime) {

	nearbyObjects = std::move(GetNearbyObjectsWithTag(searchTags, 500.0f));

	for (const AActor* actor : nearbyObjects) {
		if (actor->ActorHasTag(FName("LandTemple"))) {
			if (GEngine)
			{
				//FString Message = FString::Printf(TEXT("��⵽ %d ��LandTem����"), 1);

				//GEngine->AddOnScreenDebugMessage(
				//	-1,                    // Key (-1��ʾ�Զ�����)
				//	1.0f,                  // ��ʾʱ��(��)
				//	FColor::Green,         // ��ɫ
				//	Message                // ��������
				//);
			}
		}
	}
}

void Ablack_moneyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	characterConfig->WriteConfigData();
	ACharacter::EndPlay(EndPlayReason);
}
TArray<AActor*> Ablack_moneyCharacter::GetNearbyObjectsWithTag(TArray<FName> tagNames, float radius) const {

	TArray<AActor*> result;

	UWorld* world = GetWorld();
	FVector sphereCenter = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = {
			UEngineTypes::ConvertToObjectType(ECC_WorldStatic),
			UEngineTypes::ConvertToObjectType(ECC_WorldDynamic),
			UEngineTypes::ConvertToObjectType(ECC_Pawn),
			UEngineTypes::ConvertToObjectType(ECC_PhysicsBody)
	};

	if (!world || radius <= 0.0f) {
		return result;
	}

	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(
		world,
		sphereCenter,
		radius,
		objectTypes,
		AActor::StaticClass(),  // ֻ���Actor
		{},
		OverlappingActors
	);

	// �������ض�Tag��Actor
	for (AActor* Actor : OverlappingActors)
	{
		for (const FName tagName : tagNames) {
			if (Actor && Actor->ActorHasTag(tagName))
			{
				result.Add(Actor);
			}
		}
	}

	return result;
}

void Ablack_moneyCharacter::ChangeMusic(FName musicName) {

	AudioComponent->FadeOut(0.5f, 0.0f);

	FString Path = FString::Printf(TEXT("/Game/Music/%s.%s"),
		*musicName.ToString(),
		*musicName.ToString());

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		FSoftObjectPath(Path),
		FStreamableDelegate::CreateLambda([this, Path]()
			{
				nextBackGroundMusic = Cast<USoundBase>(FSoftObjectPath(Path).ResolveObject());
				if (nextBackGroundMusic)
				{
					AudioComponent->SetSound(nextBackGroundMusic);
					AudioComponent->Play();
					AudioComponent->FadeIn(0.5f, 0.0f);
					BackGroundMusic = nextBackGroundMusic;
					nextBackGroundMusic = nullptr;
				}
			})
	);
}

const UCharacterConfig* Ablack_moneyCharacter::ShareCharacterConfig() {
	return characterConfig;
}

void Ablack_moneyCharacter::TriggerNearByInteractions() {

	AActor* firstObject = *nearbyInteraction.begin();

	if (firstObject->IsA(TSubclassOf<ATools>())) {
		Cast<Ublack_moneyGameInstance>(GetGameInstance())->GetEventCenter()->GetTools(firstObject,1);
	}
	else if (firstObject->IsA(ALandTemple::StaticClass())) {

	}
	nearbyInteraction.Remove(firstObject);
	firstObject->Destroy();
}