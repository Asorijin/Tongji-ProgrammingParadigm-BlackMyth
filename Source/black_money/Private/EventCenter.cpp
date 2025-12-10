// Fill out your copyright notice in the Description page of Project Settings.


#include "EventCenter.h"
#include <Kismet/GameplayStatics.h>
UEventCenter::UEventCenter() {
	
}

void UEventCenter::GenerateMonster() {

}
void UEventCenter::ChangeBGM() {

}
void UEventCenter::PlayMusic(std::string musicName) {

}

void UEventCenter::MakeDamage(TSubclassOf<AActor> makeDamager, TSubclassOf<AActor> takeDamager, float damageNumber) {

}

void UEventCenter::UseTools(TArray<int>* attributeVector) {

}
void UEventCenter::GetTools(TSubclassOf<ATools>* tool,int toolNumber) {
    //道具指针应由角色属性类持有，角色属性类由于全局唯一，由GameInstance持有
    //怪物掉落一个有模型的实体ATools，属性类持有的作为不可见模型实际挂载在游戏中
    //这里检验道具属于哪一种道具，向属性类持有的道具数量+1后，销毁实体模型
}
//需要先实现装备结构体
void UEventCenter::ChangeEquipment() {

}

void UEventCenter::SwitchToLevel(const FString& LevelName, FVector SpawnLocation) {
    //目前为止仅设定了关卡和出生点，还需要在角色BeginPlay中调用移动，才能将角色移动过去
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