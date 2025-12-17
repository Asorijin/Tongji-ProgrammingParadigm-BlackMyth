// Fill out your copyright notice in the Description page of Project Settings.

#include "EventCenter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "ToolHp.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Engine/DamageEvents.h" 

// 前向声明，避免循环依赖
class Ablack_moneyCharacter;

UEventCenter::UEventCenter() {
	
}

void UEventCenter::GenerateMonster() {

}

float UEventCenter::MakeDamage(
	AActor* DamagedActor,
	float DamageAmount,
	AController* EventInstigator,
	AActor* DamageCauser,
	TSubclassOf<UDamageType> DamageTypeClass)
{
	if (!DamagedActor || DamageAmount <= 0.f)
	{
		return 0.f;
	}

	// 如果没传控制器，而 DamageCauser 是 Pawn，就自动取其 Controller
	if (!EventInstigator && DamageCauser)
	{
		if (APawn* PawnCauser = Cast<APawn>(DamageCauser))
		{
			EventInstigator = PawnCauser->GetController();
		}
	}

	
	FDamageEvent DamageEvent;
	// 设置伤害类型
	DamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());

	// 调用 Actor 的 TakeDamage
	const float ActualDamage = DamagedActor->TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser);

	// 输出日志
	if (ActualDamage > 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("EventCenter::MakeDamage - %s took %f damage from %s"),
			*DamagedActor->GetName(),
			ActualDamage,
			DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));
	}

	return ActualDamage;
}
void UEventCenter::UseTools(AActor* tool) {

}

void UEventCenter::GetTools(AActor* tool, int toolNumber) {
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

	if (UWorld* World = GetWorld())
	{
		// 获取当前关卡的短名称
		FString CurrentLevelName = GetWorld()->GetMapName();
		CurrentLevelName = FPaths::GetBaseFilename(CurrentLevelName);

		// 比较目标关卡名和当前关卡名
		if (CurrentLevelName.Equals(LevelName, ESearchCase::IgnoreCase))
		{
			// 已经在目标关卡，只需更新生成位置
			pawnLastLocation = SpawnLocation;
			UE_LOG(LogTemp, Log, TEXT("Already in level '%s', updated spawn location."), *LevelName);
			return;
		}

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
