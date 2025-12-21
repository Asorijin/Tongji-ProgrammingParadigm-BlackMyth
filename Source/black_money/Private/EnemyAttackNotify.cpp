// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAttackNotify.h"
#include "BaseEnemy.h"

UEnemyAttackNotify::UEnemyAttackNotify()
{
}

void UEnemyAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	// 尝试转换为BaseEnemy
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(Owner);
	if (!Enemy)
	{
		return;
	}

	// 调用怪物的攻击判定方法
	Enemy->PerformAttack();
}

FString UEnemyAttackNotify::GetNotifyName_Implementation() const
{
	return TEXT("EnemyAttack");
}

