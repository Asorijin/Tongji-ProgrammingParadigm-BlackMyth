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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// 创建检测球体组件
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(500.0f);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	// 设置可视化
	DetectionSphere->SetHiddenInGame(false);

	//创建音乐组件
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
		// 绑定 Ctrl -> Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::Dodge);

		//绑定攻击动作
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
// 闪避功能
void Ablack_moneyCharacter::Dodge()
{
	// 检查是否可以闪避
	if (!CanDodge())
	{
		return;
	}

	// 计算闪避方向：优先使用最近的移动输入方向；若无，则使用角色前向
	FVector DodgeDirection = GetLastMovementInputVector().GetSafeNormal();
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = GetActorForwardVector();
	}

	// 仅保留水平分量，避免 Z 轴抬升/下坠影响
	DodgeDirection.Z = 0.0f;
	DodgeDirection = DodgeDirection.GetSafeNormal();

	// 应用冲量进行闪避（覆盖 XY 速度，但不覆盖 Z）
	LaunchCharacter(DodgeDirection * DodgeStrength, /*bXYOverride*/ true, /*bZOverride*/ false);

	// 设置状态
	bIsDodging = true;
	bInvulnerableDuringDodge = true;

	// 定时结束闪避
	GetWorldTimerManager().SetTimer(
		DodgeTimerHandle,
		this,
		&Ablack_moneyCharacter::EndDodge,
		DodgeDuration,
		/*bLoop*/ false
	);
}

bool Ablack_moneyCharacter::CanDodge() const
{
	// 不在闪避中且拥有控制器和移动组件
	if (bIsDodging || Controller == nullptr || GetCharacterMovement() == nullptr)
	{
		return false;
	}

	// 如需限定地面闪避可启用：
	// return GetCharacterMovement()->IsMovingOnGround();
	return true;
}

void Ablack_moneyCharacter::EndDodge()
{
	bIsDodging = false;
	bInvulnerableDuringDodge = false;

	// 清理定时器
	if (GetWorldTimerManager().IsTimerActive(DodgeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
	}
}


void Ablack_moneyCharacter::Attack()
{
	// 闪避中不能攻击
	if (bIsDodging)
	{
		return;
	}

	// 第一击：当前没有连击
	if (ComboIndex <= 0 || !AttackMontage)
	{
		ComboIndex = 1;
		bIsAttacking = true;
		bQueuedNextCombo = false;
		bCanQueueNextCombo = false;

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName(TEXT("Attack1")), AttackMontage);
		}

		// 这里可以不再用 ComboResetTime 计时器，改用 AnimNotify 结束时 ResetCombo
		return;
	}

	// 后续攻击：当前正在播某一段
	// 如果还没到可连击窗口，只记下“排队”
	if (!bCanQueueNextCombo)
	{
		bQueuedNextCombo = true;
		return;
	}

	// 已经到可连击窗口，可以切到下一段
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		// 计算下一个段数
		int32 NextComboIndex = ComboIndex + 1;
		if (NextComboIndex > 4)
		{
			NextComboIndex = 1;
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

		if (AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			ComboIndex = NextComboIndex;
			bQueuedNextCombo = false;
			bCanQueueNextCombo = false;

			AnimInstance->Montage_JumpToSection(NextSectionName, AttackMontage);
		}
	}
}
void Ablack_moneyCharacter::EnableComboWindow()
{
	bCanQueueNextCombo = true;

	// 如果玩家已经在本段期间提前按过攻击键，则立即切到下一段
	if (bQueuedNextCombo)
	{
		Attack(); // 复用 Attack 里的“跳到下一段”逻辑
	}
}

void Ablack_moneyCharacter::OnAttackSectionEnded()
{
	// 段尾再检查一次：如果还有排队并且允许连击，就再跳一次
	if (bQueuedNextCombo && bCanQueueNextCombo)
	{
		Attack();
		return;
	}

	// 否则这套连击结束，全部重置
	ComboIndex = 0;
	bIsAttacking = false;
	bQueuedNextCombo = false;
	bCanQueueNextCombo = false;
}

void Ablack_moneyCharacter::BeginPlay() {

	Super::BeginPlay();
	
	characterConfig = NewObject<UCharacterConfig>();
	characterConfig->Initialize();

}

void Ablack_moneyCharacter::Tick(float deltaTime) {

	nearbyObjects = std::move(GetNearbyObjectsWithTag(searchTags, 500.0f));

	for (const AActor* actor : nearbyObjects) {
		if (actor->ActorHasTag(FName("LandTemple"))) {
			if (GEngine)
			{
				FString Message = FString::Printf(TEXT("检测到 %d 个LandTem物体"), 1);

				GEngine->AddOnScreenDebugMessage(
					-1,                    // Key (-1表示自动分配)
					1.0f,                  // 显示时间(秒)
					FColor::Green,         // 颜色
					Message                // 文字内容
				);
			}
		}
	}
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
		AActor::StaticClass(),  // 只检测Actor
		{},
		OverlappingActors
	);

	// 过滤有特定Tag的Actor
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