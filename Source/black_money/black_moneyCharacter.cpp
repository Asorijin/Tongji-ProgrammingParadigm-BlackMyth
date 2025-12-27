#include "black_moneyCharacter.h"
#include "black_moneyGameMode.h"
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
#include "ToolHp.h"
#include "ToolMp.h"
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
	CameraBoom->bDoCollisionTest = false; 

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// 球形检测范围
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(500.0f);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	// 武器碰撞盒
	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	WeaponHitBox->InitBoxExtent(FVector(10.f, 30.f, 10.f));

	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);    
	WeaponHitBox->SetCollisionObjectType(ECC_Pawn);
	WeaponHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);  //

	WeaponHitBox->SetHiddenInGame(false);//显示碰撞盒，方便调试

	// 显示检测范围，方便调试
	DetectionSphere->SetHiddenInGame(false);

	//音乐组件
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

		// 绑定 鼠标左键 -> Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::Attack);
		//绑定Q键->EarthQuake
		EnhancedInputComponent->BindAction(SkillEarthQuakeAction,ETriggerEvent::Started,this,&Ablack_moneyCharacter::CastEarthQuake);
		//绑定F键->拾取道具
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::TriggerNearByInteractions);
		// 绑定ESC和P键到暂停处理函数
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::HandlePauseInput);
		//绑定1键使用血量道具
		EnhancedInputComponent->BindAction(UseHealthItemAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::UseToolHp);
		//绑定2键使用蓝量道具
		EnhancedInputComponent->BindAction(UseManaItemAction, ETriggerEvent::Started, this, &Ablack_moneyCharacter::UseToolMp);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void Ablack_moneyCharacter::HandlePauseInput()
{
	// 获取当前GameMode并调用暂停函数
	if (UWorld* World = GetWorld())
	{
		Ablack_moneyGameMode* GameMode = Cast<Ablack_moneyGameMode>(World->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->TogglePause(); // 调用GameMode中实现的暂停逻辑
		}
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

	// 应用闪避冲量
	LaunchCharacter(DodgeDirection * DodgeStrength, /*bXYOverride*/ true, /*bZOverride*/ false);


	bInvulnerableDuringDodge = true;

	// 播放闪避动画蒙太奇
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
	// 已经在闪避中，或无效控制器，或无效移动组件时，不能闪避
	if (bIsDodging || Controller == nullptr || GetCharacterMovement() == nullptr)
	{
		return false;
	}

	// 只能在地面上闪避
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

	//初始攻击
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

	//如果不是连击，直接返回
	if (!bCanQueueNextCombo)
	{
		return;
	}

	// 连击逻辑
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
	// 允许连击输入
	bCanQueueNextCombo = true;
}

void Ablack_moneyCharacter::OnAttackSectionEnded()
{
	// 连击结束，重置状态
	bCanQueueNextCombo = false;

	// 重置连击索引
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
	//没在攻击状态或碰撞盒无效时，直接返回
	if (!bIsAttacking || !WeaponHitBox ||
		WeaponHitBox->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		return;
	}
	//攻击对象无效或是自己时，直接返回
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	//已经命中该对象时，直接返回，防止重复伤害
	if (AlreadyHitActors.Contains(OtherActor))
	{
		return;
	}
	AlreadyHitActors.Add(OtherActor);


	// 触发伤害事件
	if (EventCenter && characterConfig)
	{
		const float DamageValue = static_cast<float>(characterConfig->_attack);

		EventCenter->MakeDamage(
			OtherActor,      // 受击对象
			DamageValue,     // 伤害值
			GetController(), // Instigator
			this           // 伤害来源
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
bool Ablack_moneyCharacter::CanCastEarthQuake() const
{
	// 死亡/闪避/受击时不能放
	if (bIsDead || bIsDodging || bIsTakingDamage)
	{
		return false;
	}

	// 没有配置，直接不行
	if (!characterConfig)
	{
		return false;
	}

	// 冷却中
	if (EarthQuakeCooldownRemaining > 0.0f)
	{
		return false;
	}

	// 蓝量不足
	if (characterConfig->_mp < EarthQuakeManaCost)
	{
		return false;
	}

	return true;
}

void Ablack_moneyCharacter::CastEarthQuake()
{
	if (!CanCastEarthQuake())
	{
		if (GEngine)
		{
			FString Reason = TEXT("Cannot cast EarthQuake");
			if (!characterConfig)
			{
				Reason = TEXT("No characterConfig");
			}
			else if (EarthQuakeCooldownRemaining > 0.0f)
			{
				Reason = FString::Printf(TEXT("EarthQuake cooldown: %.1fs"), EarthQuakeCooldownRemaining);
			}
			else if (characterConfig->_mp < EarthQuakeManaCost)
			{
				Reason = FString::Printf(TEXT("MP not enough: %d / %d"),
					characterConfig->_mp,
					characterConfig->GetMaxMp());
			}
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, Reason);
		}
		return;
	}
	// 标记进入“释放技能”状态
	bIsCastingSkill = true;
	// 扣蓝
	characterConfig->_mp -= EarthQuakeManaCost;
	characterConfig->_mp = FMath::Clamp(characterConfig->_mp, 0, characterConfig->GetMaxMp());

	// 冷却开始
	EarthQuakeCooldownRemaining = EarthQuakeCooldown;

	// 播放技能蒙太奇
	if (EarthQuakeMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// 释放技能时，可以视情况停止普通攻击蒙太奇
			if (bIsAttacking && AttackMontage && AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				AnimInstance->Montage_Stop(0.1f, AttackMontage);
				bIsAttacking = false;
				ComboIndex = 0;
				bCanQueueNextCombo = false;
			}

			AnimInstance->Montage_Play(EarthQuakeMontage, 1.0f);
		}
	}

	if (GEngine)
	{
		const FString Msg = FString::Printf(TEXT("Cast EarthQuake, MP=%d/%d"),
			characterConfig->_mp,
			characterConfig->GetMaxMp());
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, Msg);
	}

}
//实际技能伤害及效果
void Ablack_moneyCharacter::DoEarthQuakeDamage()
{
	if (!EventCenter || !characterConfig)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 技能半径，可在蓝图里改，这里给个默认值
	const float Radius = 400.0f;
	const FVector Center = GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(
		World,
		Center,
		Radius,
		ObjectTypes,
		AActor::StaticClass(),
		TArray<AActor*>(), // 不忽略
		OverlappingActors
	);

	// 技能伤害倍率，例如普通攻击伤害 * 2
	const float BaseAttack = static_cast<float>(characterConfig->_attack);
	const float DamageValue = BaseAttack * 2.0f;

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || Actor == this)
		{
			continue;
		}

		// 只打带 Enemy 标签的
		if (!Actor->ActorHasTag(FName("Enemy")))
		{
			continue;
		}

		EventCenter->MakeDamage(
			Actor,
			DamageValue,
			GetController(),
			this);

		if (GEngine)
		{
			const FString HitMsg = FString::Printf(TEXT("EarthQuake hit %s, Damage=%.1f"),
				*Actor->GetName(), DamageValue);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, HitMsg);
		}
	}
}
void Ablack_moneyCharacter::BeginPlay() {
	Super::BeginPlay();
	// 获取事件中心
	if (Ublack_moneyGameInstance* GI = Cast<Ublack_moneyGameInstance>(GetGameInstance()))
	{
		EventCenter = GI->GetEventCenter();
	}
	
	characterConfig = NewObject<UCharacterConfig>();
	characterConfig->Initialize();
	
	this->SetActorLocation(EventCenter->GetSpawnLocation());
	FString PIEMapName = GetWorld()->GetMapName();
	FString RawMapName = PIEMapName;

	// 绑定武器碰撞盒重叠事件
	if (WeaponHitBox)
	{
		WeaponHitBox->OnComponentBeginOverlap.AddDynamic(this,&Ablack_moneyCharacter::OnWeaponHitBoxBeginOverlap);
	}

	// 获取GameMode并触发状态栏显示 CharacterMenu
	if (Ablack_moneyGameMode* GameMode = Cast<Ablack_moneyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->ShowStatusBar();  // 启动菜单关闭后，才显示状态栏
	}

}

void Ablack_moneyCharacter::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	// ===== 蓝量回复=====
	if (characterConfig) {
		const int32 MaxMp = characterConfig->GetMaxMp(); if (MaxMp > 0 && ManaRegenPerSecond > 0.0f) {
			const float RegenThisFrame = ManaRegenPerSecond * deltaTime;
			MpRegenAccumulator += RegenThisFrame;

			// 只要累计超过 1，就转为整数加到 mp
			const int32 IntGain = static_cast<int32>(MpRegenAccumulator);
			if (IntGain > 0)
			{
				MpRegenAccumulator -= IntGain; // 剩余小数部分保留到下一帧
				characterConfig->_mp += IntGain;
				characterConfig->_mp = FMath::Clamp(characterConfig->_mp, 0, MaxMp);
			}
		}
	}
	// ===== 技能冷却计时 =====
	if (EarthQuakeCooldownRemaining > 0.0f)
	{
		EarthQuakeCooldownRemaining -= deltaTime;
		if (EarthQuakeCooldownRemaining < 0.0f)
		{
			EarthQuakeCooldownRemaining = 0.0f;
		}
	}
	if (!GEngine || !characterConfig)
	{
		return;
	}
	//  受击状态兜底 ,防止僵直
	if (bIsTakingDamage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (!HitMontage || !AnimInstance->Montage_IsPlaying(HitMontage))
			{
				// 受击蒙太奇已经不在播放了，就强制结束受击状态
				EndHit();
			}
		}
		else
		{
			EndHit();
		}
	}
	const int32 CurrentMp = characterConfig->_mp;
	const int32 MaxMp = characterConfig->GetMaxMp();

	const FString Msg = FString::Printf(TEXT("MP = %d / %d"), CurrentMp, MaxMp);

	// Key 用 1，避免刷屏堆很多条
	GEngine->AddOnScreenDebugMessage(
		/*Key*/ 1,
		/*Time*/ 0.1f,
		FColor::Cyan,
		Msg);

}

void Ablack_moneyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (!bIsDead)
	{
		if (IsValid(characterConfig))
		{
			characterConfig->WriteConfigData();
		}

		if (IsValid(EventCenter))
		{
			EventCenter->WriteLastState();
		}
		bIsDead = false; 
	}
	nearbyInteraction.Empty();
	ACharacter::EndPlay(EndPlayReason);
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
	AActor* firstObject;
	UE_LOG(LogTemp, Log, TEXT("Now Object Name: '%d' "), nearbyInteraction.Num());
	if (nearbyInteraction.Num())
		firstObject = *nearbyInteraction.begin();
	else
		return;
	
	if (firstObject->IsA(AToolHp::StaticClass())) {
		nearbyInteraction.Remove(firstObject);
		Cast<Ublack_moneyGameInstance>(GetGameInstance())->GetEventCenter()->GetTools(firstObject,1);
	}
	else if (firstObject->IsA(AToolMp::StaticClass())) {
		nearbyInteraction.Remove(firstObject);
		Cast<Ublack_moneyGameInstance>(GetGameInstance())->GetEventCenter()->GetTools(firstObject, 1);
	}
	else if (firstObject->IsA(ALandTemple::StaticClass())) {

		FString ActorName = firstObject->GetActorNameOrLabel();
		UE_LOG(LogTemp, Log, TEXT("Now Object Name: '%s' "), *ActorName);

		if (ActorName == FString("JumpToLevel1")) {
			EventCenter->SetLevelAndLocation(TEXT("/Game/ThirdPerson/Maps/Midgardr_Free"), FVector(-330,3500,150));
		}
		else if (ActorName == FString("JumpToLevel2")) {
			EventCenter->SetLevelAndLocation(TEXT("/Game/ThirdPerson/Maps/Demo_Scene"),FVector(1200,-3300,150));
		}
		EventCenter->SwitchToLevel();
	}
	
}

void Ablack_moneyCharacter::UseToolHp() {
	if (EventCenter->UseTools(AToolHp::StaticClass())) {
		
		if (characterConfig->_hp + 10 > characterConfig->GetMaxHp()) {
			characterConfig->_hp =characterConfig->GetMaxHp();
		}
		else {
			characterConfig->_hp += 10;
		}
	}
}

void Ablack_moneyCharacter::UseToolMp() {
	if (EventCenter->UseTools(AToolMp::StaticClass())) {
		if (characterConfig->_mp + 10 > characterConfig->GetMaxMp()) {
			characterConfig->_mp = characterConfig->GetMaxMp();
		}
		else {
			characterConfig->_mp += 10;
		}
	}
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
				 bIsAttacking = false;
				 ComboIndex = 0;
				 bCanQueueNextCombo = false;
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

		FTimerDelegate TimerCallback;
		FTimerHandle DelayTimerHandle;
		TimerCallback.BindLambda([this]()
			{
				Ublack_moneyGameInstance* GI = Cast<Ublack_moneyGameInstance>(GetGameInstance());
				GI->bIsFirstLaunch = true;
				GI->GetEventCenter()->SetLevelAndLocation(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap"), FVector());
				GI->GetEventCenter()->SwitchToLevel();
			});

		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, TimerCallback, 5.0f, false);
		
	}

