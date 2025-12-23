// Fill out your copyright notice in the Description page of Project Settings.

#include "EventCenter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "ToolHp.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Json.h"
#include "Misc/FileHelper.h"
#include "Engine/DamageEvents.h" 

// 前向声明，避免循环依赖
class Ablack_moneyCharacter;

UEventCenter::UEventCenter() {
	
}

void UEventCenter::PostInitProperties() {
	UObject::PostInitProperties();

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
		if (toolsNumber.hpTools)
		{
			toolsNumber.hpTools += 1;
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

void UEventCenter::WriteLastState() {
	// 1. 淇濆瓨 PawnLastLocation (鎷嗘垚 X/Y/Z)
	TSharedPtr<FJsonObject> LocObj = MakeShareable(new FJsonObject);
	LocObj->SetNumberField("X", pawnLastLocation.X);
	LocObj->SetNumberField("Y", pawnLastLocation.Y);
	LocObj->SetNumberField("Z", pawnLastLocation.Z);
	LocObj->SetObjectField("PawnLastLocation", LocObj);

	// 2. 淇濆瓨 LevelName
	LocObj->SetStringField("LevelName", levelName);

	// 3. 淇濆瓨 ToolsNumber
	TSharedPtr<FJsonObject> ToolsObj = MakeShareable(new FJsonObject);
	ToolsObj->SetNumberField("hpTools", toolsNumber.hpTools);
	ToolsObj->SetNumberField("mpTools", toolsNumber.mpTools);
	LocObj->SetObjectField("ToolsNumber", ToolsObj);


	FString outputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(LocObj.ToSharedRef(), Writer);

	if (!FFileHelper::SaveStringToFile(outputString, *filePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save JSON to file: %s"), *filePath);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully saved character config to: %s"), *filePath);
	}
}

void UEventCenter::ReadLastState() {
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Save file not found: %s"), *filePath);
	}

	FString JsonContent;
	if (!FFileHelper::LoadFileToString(JsonContent, *filePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *filePath);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonContent);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON in file: %s"), *filePath);
	}

	if (JsonObject->HasField("PawnLastLocation") && JsonObject->GetObjectField("PawnLastLocation").IsValid())
	{
		TSharedPtr<FJsonObject> LocObj = JsonObject->GetObjectField("PawnLastLocation");
		pawnLastLocation.X = LocObj->GetNumberField("X");
		pawnLastLocation.Y = LocObj->GetNumberField("Y");
		pawnLastLocation.Z = LocObj->GetNumberField("Z");
	}

	if (JsonObject->HasField("LevelName"))
	{
		levelName = JsonObject->GetStringField("LevelName");
	}

	if (JsonObject->HasField("ToolsNumber") && JsonObject->GetObjectField("ToolsNumber").IsValid())
	{
		TSharedPtr<FJsonObject> ToolsObj = JsonObject->GetObjectField("ToolsNumber");
		toolsNumber.hpTools = static_cast<int32>(ToolsObj->GetNumberField("hpTools"));
		toolsNumber.mpTools = static_cast<int32>(ToolsObj->GetNumberField("mpTools"));
	}

	UE_LOG(LogTemp, Log, TEXT("Successfully loaded save data from: %s"), *filePath);
}