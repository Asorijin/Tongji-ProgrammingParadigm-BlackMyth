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

void UEventCenter::MakeDamage(TSubclassOf<AActor*> makeDamager, TArray<TSubclassOf<AActor*>> takeDamager, int damageNumber) {
	
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
<<<<<<< HEAD
	pawnLastLocation = SpawnLocation;
=======
    pawnLastLocation = SpawnLocation;
>>>>>>> f45d162c5e976b269c667c2381fcf4542a1f5877

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
<<<<<<< HEAD
	return pawnLastLocation;
=======
    return pawnLastLocation;
>>>>>>> f45d162c5e976b269c667c2381fcf4542a1f5877
}
