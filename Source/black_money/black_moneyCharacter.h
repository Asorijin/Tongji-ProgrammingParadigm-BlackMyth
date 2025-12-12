// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TimerManager.h"
#include <Components/SphereComponent.h>
#include <Components/AudioComponent.h>
#include "CharacterConfig.h"
#include "black_moneyCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class Ablack_moneyCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	// 添加 DodgeAction 指针
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	
public:
	Ablack_moneyCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	/** 闪避功能（按 Ctrl 触发） */
	void Dodge();

	/** 闪避条件检查 */
	bool CanDodge() const;

	/** 闪避结束回调 */
	void EndDodge();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* DetectionSphere;

	//控制音乐\循环音效
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* BackGroundMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* nextBackGroundMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterConfig* characterConfig;

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void BeginPlay() override;

	void Tick(float deltaTime) override;
private:
	/** 闪避强度（水平冲量） */
	UPROPERTY(EditAnywhere, Category = "Dodge")
	float DodgeStrength = 1200.0f;

	/** 闪避持续时间（免伤持续时间） */
	UPROPERTY(EditAnywhere, Category = "Dodge")
	float DodgeDuration = 0.35f;

	/** 是否处于闪避中 */
	UPROPERTY(VisibleAnywhere, Category = "Dodge")
	bool bIsDodging = false;

	/** 闪避期间免伤标记 */
	UPROPERTY(VisibleAnywhere, Category = "Dodge")
	bool bInvulnerableDuringDodge = true;

	/** 定时器句柄，用于结束闪避 */
	FTimerHandle DodgeTimerHandle;

	//根据指定TAG获取周围物体
	TArray<AActor*> GetNearbyObjectsWithTag(TArray<FName> tagNames, float radius) const;

	//获取到的指定TAG物体
	TArray<AActor*> nearbyObjects;

	//目标物体的TAG
	TArray<FName> searchTags = {FName("LandTemple")};

	//改变音乐
	void ChangeMusic(FName musicName);

	const UCharacterConfig* ShareCharacterConfig();
};

