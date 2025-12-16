// Fill out your copyright notice in the Description page of Project Settings.

#include "EventCenter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

// 前向声明，避免循环依赖
class Ablack_moneyCharacter;

UEventCenter::UEventCenter() {
	
}

void UEventCenter::GenerateMonster() {

}

void UEventCenter::MakeDamage(TSubclassOf<AActor> makeDamager, TSubclassOf<AActor> takeDamager, int damageNumber) {
	// 注意：这个方法的参数设计不太合理，应该使用AActor*指针
	// 但为了保持接口一致性，这里先实现基础功能
	// 推荐使用MakeDamageToActor方法
	
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("EventCenter::MakeDamage: No valid world!"));
		return;
	}

	// 查找所有受击者（takeDamager类型的Actor）
	TArray<AActor*> TakeDamagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), takeDamager, TakeDamagerActors);

	// 查找所有伤害者（makeDamager类型的Actor）
	TArray<AActor*> MakeDamagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), makeDamager, MakeDamagerActors);

	// 对每个受击者造成伤害
	for (AActor* TakeDamagerActor : TakeDamagerActors)
	{
		// 找到最近的伤害者（简化处理，实际应该根据攻击范围等判断）
		AActor* ClosestDamager = nullptr;
		float ClosestDistance = MAX_FLT;

		for (AActor* MakeDamagerActor : MakeDamagerActors)
		{
			if (MakeDamagerActor && MakeDamagerActor != TakeDamagerActor)
			{
				float Distance = FVector::Dist(MakeDamagerActor->GetActorLocation(), 
					TakeDamagerActor->GetActorLocation());
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestDamager = MakeDamagerActor;
				}
			}
		}

		// 使用新的方法处理伤害
		MakeDamageToActor(ClosestDamager, TakeDamagerActor, damageNumber);
	}
}

void UEventCenter::MakeDamageToActor(AActor* DamageCauser, AActor* DamageReceiver, int32 DamageAmount)
{
	if (!DamageReceiver)
	{
		UE_LOG(LogTemp, Warning, TEXT("EventCenter::MakeDamageToActor: DamageReceiver is null!"));
		return;
	}

	if (DamageAmount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EventCenter::MakeDamageToActor: DamageAmount is invalid: %d"), DamageAmount);
		return;
	}

	// 检查是否是怪物
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(DamageReceiver))
	{
		// 对怪物造成伤害（通过事件中心解耦）
		Enemy->ReceiveDamage(DamageAmount, DamageCauser);
		UE_LOG(LogTemp, Log, TEXT("EventCenter: Made %d damage to enemy %s"), DamageAmount, *Enemy->GetName());
	}
	// 检查是否是玩家角色（使用字符串比较避免包含头文件）
	else if (DamageReceiver && DamageReceiver->IsA<APawn>())
	{
		// 玩家受击处理（如果需要）
		// 可以通过CharacterConfig来处理玩家受击
		UE_LOG(LogTemp, Log, TEXT("EventCenter: Made %d damage to player %s"), DamageAmount, *DamageReceiver->GetName());
		// TODO: 实现玩家受击逻辑
		// 注意：如果需要调用玩家的特定方法，需要包含 black_moneyCharacter.h
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EventCenter::MakeDamageToActor: Unknown damage receiver type: %s"), 
			*DamageReceiver->GetClass()->GetName());
	}
}

void UEventCenter::UseTools(TArray<int>* attributeVector) {

}

void UEventCenter::GetTools(AActor* tool, int toolNumber) {
	// 工具指针应该通过EventCenter处理
	// 这里应该是一个工具模型的实例ATools，工具中的行为由工具模型实际使用在游戏中
	// 如果拾取的是同一种工具，工具中的工具数量+1，否则实例化模型
    if (tool->IsA(AToolHp::StaticClass())) {
		if (toolsNumber)
		{
        toolsNumber->hpTools += 1;
		}
    }
    else {
		// 其他类型工具的处理
    }
    tool->Destroy();
}

void UEventCenter::ChangeEquipment() {
	// 需要实现装备结构
}

void UEventCenter::SwitchToLevel(const FString& LevelName, FVector SpawnLocation) {
	// 目前只设定了关卡和生成点，还需要在角色BeginPlay中调用移动才能将角色移动到新位置
    pawnLastLocation = SpawnLocation;

    if (UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevel(World, FName(*LevelName));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SwitchToLevel: No valid world!"));
    }
}

const FVector UEventCenter::GetSpawnLocation() {
    return pawnLastLocation;
}
