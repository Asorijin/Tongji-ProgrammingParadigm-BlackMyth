// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "wukong_AnimNotify.generated.h"

/**
 * 
 */
 // 通知类型枚举：用一个 Notify 类囊括多种功能
UENUM(BlueprintType)
enum class EStatusType : uint8
{
	OpenComboWindow    UMETA(DisplayName = "Open Combo Window"),     // 开启连击判定
	EndAttackSection   UMETA(DisplayName = "End Attack Section"),    // 当前攻击段结束
	EnableHit          UMETA(DisplayName = "Enable Hit"),            // 开启伤害判定
	DisableHit         UMETA(DisplayName = "Disable Hit"),            // 关闭伤害判定
	EndDodge           UMETA(DisplayName = "End Dodge"),            // 闪避结束
	EndHit UMETA(DisplayName = "End Hit")                          // 受击结束

};
UCLASS()
class BLACK_MONEY_API Uwukong_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	// 在蒙太奇 / 动画里可以选择具体通知类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	EStatusType NotifyType =EStatusType::OpenComboWindow;
	//在编辑器中显示通知名称
	virtual FString GetNotifyName_Implementation() const override
	{
		switch (NotifyType)
		{
		case EStatusType::OpenComboWindow:
			return TEXT("Open Combo Window");
		case EStatusType::EndAttackSection:
			return TEXT("End Attack Section");
		case EStatusType::EnableHit:
			return TEXT("Enable Hit");
		case EStatusType::DisableHit:
			return TEXT("Disable Hit");
		case EStatusType::EndDodge:
			return TEXT("End Dodge");
		case EStatusType::EndHit:
			return TEXT("End Hit");
		default:
			return Super::GetNotifyName_Implementation();
		}
	}
	
	// 通知触发时调用
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
