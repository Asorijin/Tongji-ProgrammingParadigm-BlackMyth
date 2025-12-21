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
	// 创建武器碰撞盒
	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	WeaponHitBox->InitBoxExtent(FVector(10.f, 30.f, 10.f));

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);    // 默认关闭
	WeaponHitBox->SetCollisionObjectType(ECC_Pawn);
	WeaponHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);  // 与 Pawn 重叠

	WeaponHitBox->SetHiddenInGame(false);//便于调试查看，最后改为true

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
void Ablack_moneyCharacter::Dodge() { 
	// 检查是否可以闪避 
	if (!CanDodge()) { return; }
	// 进入闪避状态
	bIsDodging = true;
	// 计算闪避方向：优先使用最近移动输入方向；若无，则使用角色前向
	FVector DodgeDirection = GetLastMovementInputVector().GetSafeNormal();
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = GetActorForwardVector();
	}
	
	// 仅保留水平分量
	DodgeDirection.Z = 0.0f;
	DodgeDirection = DodgeDirection.GetSafeNormal();

	// 应用冲量进行闪避（覆盖 XY 速度，但不覆盖 Z）
	LaunchCharacter(DodgeDirection * DodgeStrength, /*bXYOverride*/ true, /*bZOverride*/ false);

	bInvulnerableDuringDodge = true;

	// 播放闪避蒙太奇
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

}


void Ablack_moneyCharacter::Attack()
{
	// 闪避中不能攻击
	if (bIsDodging)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	const bool bMontagePlaying = (AnimInstance && AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage));

	// 起手条件：不在攻击中 / 连击已清零 / 没有蒙太奇 / 蒙太奇没在播
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

	// 已在攻击中：只有在连击窗口开放时，按键才生效
	if (!bCanQueueNextCombo)
	{
		return;
	}

	// 可以连击：切到下一段
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
	// 当前段到达“可连击”时间点
	bCanQueueNextCombo = true;
}

void Ablack_moneyCharacter::OnAttackSectionEnded()
{
	// 当前段彻底结束，若此时仍未连击，则整套攻击结束
	bCanQueueNextCombo = false;

	// 真正结束连击
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
	// 只在攻击状态且当前 HitBox 开启时才生效
	if (!bIsAttacking || !WeaponHitBox ||
		WeaponHitBox->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		return;
	}
	// 排除无效对象和自身
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// 一次攻击窗口内防止重复命中
	if (AlreadyHitActors.Contains(OtherActor))
	{
		return;
	}
	AlreadyHitActors.Add(OtherActor);


	// 通过 EventCenter 结算伤害
	// 通过 EventCenter 结算伤害
	if (EventCenter && characterConfig)
	{
		const float DamageValue = static_cast<float>(characterConfig->_attack);

		EventCenter->MakeDamage(
			OtherActor,      // 被伤害对象
			DamageValue,     // 伤害数值（来自配置）
			GetController(), // Instigator
			this             // 角色自己
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

	// 绑定武器碰撞重叠委托
	if (WeaponHitBox)
	{
		WeaponHitBox->OnComponentBeginOverlap.AddDynamic(this,&Ablack_moneyCharacter::OnWeaponHitBoxBeginOverlap);
	}

	// 从 GameInstance 获取 EventCenter（按你 TriggerNearByInteractions 里的用法）
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
				//FString Message = FString::Printf(TEXT("检测到 %d 个LandTem物体"), 1);

				//GEngine->AddOnScreenDebugMessage(
				//	-1,                    // Key (-1表示自动分配)
				//	1.0f,                  // 显示时间(秒)
				//	FColor::Green,         // 颜色
				//	Message                // 文字内容
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

float Ablack_moneyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 闪避无敌,直接免伤
	if (bInvulnerableDuringDodge)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 1.0f, FColor::Cyan,
				TEXT("TakeDamage blocked by dodge i-frame"));
		}
		return 0.0f;
	}
	// 标记处于受击中
	bIsTakingDamage = true;
	// 没有配置的话，走父类默认逻辑
	if (!characterConfig)
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	// 伤害计算逻辑 

	//  把角色配置里的 hp、防御拿出来
	const int32 Defence = characterConfig->_defence;  
	const float FinalDamage = FMath::Max(DamageAmount - Defence, 1.0f);

	// 扣血
	characterConfig->_hp -= static_cast<int32>(FinalDamage);
	characterConfig->_hp = FMath::Clamp(characterConfig->_hp, 0, characterConfig->GetMaxHp());

	if (GEngine)
	{
		const FString Msg = FString::Printf(
			TEXT("TakeDamage: %.1f, HP=%d/%d"),
			FinalDamage,
			characterConfig->_hp,
			characterConfig->GetMaxHp());
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Msg);
	}
	// 播放受击蒙太奇
	if (characterConfig->_hp > 0 && HitMontage && !bIsDodging)
	{

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// 如果正在攻击，先停止攻击蒙太奇
			 if (bIsAttacking)
			 {
			     AnimInstance->Montage_Stop(0.1f, AttackMontage);
			 }

			// 确保蒙太奇已在播放
			if (!AnimInstance->Montage_IsPlaying(HitMontage))
			{
				AnimInstance->Montage_Play(HitMontage, 1.0f);
			}

			// 根据 DamageCauser 计算方向并跳转 Section
			const FName SectionName = GetHitSectionNameForCauser(this, DamageCauser);
			AnimInstance->Montage_JumpToSection(SectionName, HitMontage);

			if (GEngine)
			{
				const FString DirMsg = FString::Printf(TEXT("Hit Section: %s"), *SectionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, DirMsg);
			}
		}
	}
	//  判断死亡
	else if (characterConfig->_hp <= 0)
	{
		HandleDeath();
	}

	// 返回实际造成的伤
	return FinalDamage;
}
	// 根据攻击者位置计算受击方向对应的 Section 名
	FName Ablack_moneyCharacter::GetHitSectionNameForCauser(const AActor* Victim, const AActor* DamageCauser)
	{
		// 默认前方受击
		FName SectionName = TEXT("Hit_Front");

		if (!Victim || !DamageCauser)
		{
			return SectionName;
		}

		const FVector VictimLocation = Victim->GetActorLocation();
		const FVector CauserLocation = DamageCauser->GetActorLocation();

		// 从受击者指向攻击者的向量（忽略 Z 高度，只看水平面）
		const FVector ToCauser = (CauserLocation - VictimLocation).GetSafeNormal2D();

		// 受击者自身的前向和右向（也只看水平面）
		const FVector Forward = Victim->GetActorForwardVector().GetSafeNormal2D();
		const FVector Right = Victim->GetActorRightVector().GetSafeNormal2D();

		// 和前/右向量做点积
		const float ForwardDot = FVector::DotProduct(Forward, ToCauser);
		const float RightDot = FVector::DotProduct(Right, ToCauser);

		// 绝对值更大的那个方向决定大类：前后 or 左右
		if (FMath::Abs(ForwardDot) >= FMath::Abs(RightDot))
		{
			// 前（攻击者大致在自己前方）
			if (ForwardDot >= 0.f)
			{
				SectionName = TEXT("Hit_Front");
			}
			// 后（攻击者在自己背后）
			else
			{
				SectionName = TEXT("Hit_Back");
			}
		}
		else
		{
			// 右（攻击者在自己右侧）
			if (RightDot >= 0.f)
			{
				SectionName = TEXT("Hit_Right");
			}
			// 左（攻击者在自己左侧）
			else
			{
				SectionName = TEXT("Hit_Left");
			}
		}

		return SectionName;
	}


	void Ablack_moneyCharacter::HandleDeath()
	{
		// 已经处理过死亡就不再重复
		if (bIsDead)
		{
			return;
		}

		bIsDead = true;

		// 停止移动
		if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
		{
			MoveComp->StopMovementImmediately();
			MoveComp->DisableMovement();
		}

		// 关闭碰撞（防止被再次打中/挡路）
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 禁用输入（玩家角色）
		if (AController* C = GetController())
		{
			if (APlayerController* PC = Cast<APlayerController>(C))
			{
				DisableInput(PC);
			}
		}

		// 播放死亡蒙太奇
		if (DeathMontage)
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				// 停掉攻击/受击蒙太奇
				if (AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
				{
					AnimInstance->Montage_Stop(0.1f, AttackMontage);
				}
				if (HitMontage && AnimInstance->Montage_IsPlaying(HitMontage))
				{
					AnimInstance->Montage_Stop(0.1f, HitMontage);
				}

				AnimInstance->Montage_Play(DeathMontage, 1.0f);
			}
		}

		// Debug 显示
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Red,
				TEXT("Player Dead"));
		}

		// 在这里触发 GameOver UI、切关、回到主菜单等
		// 例如：Cast<Ublack_moneyGameInstance>(GetGameInstance())->OnPlayerDead();
	}

