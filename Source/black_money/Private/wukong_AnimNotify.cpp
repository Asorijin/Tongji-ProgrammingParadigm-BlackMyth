// Fill out your copyright notice in the Description page of Project Settings.


#include "wukong_AnimNotify.h"
#include"black_money/black_moneyCharacter.h"
void Uwukong_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	Ablack_moneyCharacter* Character = Cast<Ablack_moneyCharacter>(Owner);
	if (!Character)
	{
		return;
	}

	switch (NotifyType)
	{
	case EStatusType::OpenComboWindow:
		// 开启连击窗口
		Character->EnableComboWindow();
		break;

	case EStatusType::EndAttackSection:
		// 当前攻击段结束
		Character->OnAttackSectionEnded();
		break;

	case EStatusType::EnableHit:
		// 开启伤害判定
		 Character->StartAttackHit();
		break;

	case EStatusType::DisableHit:
		// 关闭伤害判定
		 Character->EndAttackHit();
		break;
	case EStatusType::EndHit:
		Character->EndHit();
		break;
	case EStatusType::EndDodge:
		Character->EndDodge();
		break;

	default:
		break;
	}
}
