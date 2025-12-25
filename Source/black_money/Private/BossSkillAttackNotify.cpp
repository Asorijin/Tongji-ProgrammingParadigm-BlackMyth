// Fill out your copyright notice in the Description page of Project Settings.

#include "BossSkillAttackNotify.h"
#include "BossEnemy.h"

UBossSkillAttackNotify::UBossSkillAttackNotify()
{
}

void UBossSkillAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	// 获取拥有该网格的Actor
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	// 尝试转换为BossEnemy
	ABossEnemy* BossEnemy = Cast<ABossEnemy>(Owner);
	if (!BossEnemy)
	{
		return;
	}

	// 调用Boss的技能攻击判定函数
	BossEnemy->PerformSkillAttack();
}

FString UBossSkillAttackNotify::GetNotifyName_Implementation() const
{
	return TEXT("BossSkillAttack");
}

