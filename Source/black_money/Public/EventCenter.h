// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventCenter.generated.h"

USTRUCT(BlueprintType)
struct FToolsNumber {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 hpTools = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mpTools = 0;
};

/**
 * 事件中心�?
 * 负责管理游戏中的各种事件，实现系统间的解�?
 */
UCLASS()
class BLACK_MONEY_API UEventCenter : public UObject
{
	GENERATED_BODY()

private:
	// 角色最后位置（用于关卡切换时保存位置）

	FString filePath = FPaths::ProjectSavedDir() / TEXT("Config/GameConfigs/LastState.txt");

	FString ActorsfilePath = FPaths::ProjectSavedDir() / TEXT("Config/GameConfigs");
	// 瑙掕壊鏈€鍚庝綅缃紙鐢ㄤ簬鍏冲崱鍒囨崲鏃朵繚瀛樹綅缃級

	FVector pawnLastLocation = FVector();
	
	// 角色最后关�?
	FString levelName = TEXT("/Game/ThirdPerson/Maps/Midgardr_Free");


	// 工具数量统计
	FToolsNumber toolsNumber = FToolsNumber();


public:
	UEventCenter();

	// ���ɳ������弰����
	void GenerateActors();
	/**
	 * 切换关卡
	 * 在调用该函数前，在角色的BeginPlay中调用，设置当前位置为SpawnLocation
	 * @param LevelName 要切换到的关卡名�?
	 * @param SpawnLocation 记录的角色位�?
	 */
	void SwitchToLevel();

	/**
	 * 获取生成位置
	 * @return 返回保存的角色位�?
	 */
	const FVector GetSpawnLocation();
	void SetLevelAndLocation(FString levelName, FVector location);
	/**
	 * 造成伤害（类�?UGameplayStatics::ApplyDamage�?
	 * @param DamagedActor   被伤害对�?
	 * @param DamageAmount   伤害数�?
	 * @param EventInstigator 造成伤害的控制器（可�?nullptr�?
	 * @param DamageCauser   造成伤害�?Actor（如角色、武器）
	 * @param DamageTypeClass 伤害类型（可选，默认 UDamageType�?
	 * @return 实际造成的伤害�?
	 */
	UFUNCTION(BlueprintCallable, Category = "EventCenter|Damage")
	float MakeDamage(
		AActor* DamagedActor,
		float DamageAmount,
		AController* EventInstigator = nullptr,
		AActor* DamageCauser = nullptr,
		TSubclassOf<UDamageType> DamageTypeClass = nullptr
		 );

	/**
	 * 使用道具
	 * @param tool 道具指针
	 */
	bool UseTools(UClass* tool);

	/**
	 * 拾取道具
	 * @param tool 道具对象
	 * @param toolNumber 道具编号

	 */
	void GetTools(AActor* tool, int toolNumber);

	void WriteLastState();

	void ReadLastState();

	void ReadActorsPosition(TArray<FVector>& OutMonsterPositions, TArray<FVector>& OutTempleLandPositions, const FString& ActorFilePath);

	/**
	 * ��ȡ��ǰ���е��ߵ�������Ϣ
	 */
	UFUNCTION(BlueprintCallable, Category = "EventCenter|Tools")
	const FToolsNumber& GetToolsNumber() const { return toolsNumber; }
};
