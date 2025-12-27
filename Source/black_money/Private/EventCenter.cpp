// Fill out your copyright notice in the Description page of Project Settings.

#include "EventCenter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "ToolHp.h"
#include "ToolMp.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Json.h"
#include "Misc/FileHelper.h"
#include "Engine/DamageEvents.h" 
#include "FUIPanelManager.h"

// 前向声明，避免循环依赖
class Ablack_moneyCharacter;

UEventCenter::UEventCenter() {
	
}

void UEventCenter::GenerateActors() {

	TArray<FVector> monsterPositions;

	FString nowPath = ActorsfilePath / FPaths::GetBaseFilename(levelName) / TEXT("ActorPosition.txt");

	ReadActorsPosition(monsterPositions,nowPath);

	UClass* Monster1 = LoadClass<AActor>(
		nullptr,
		TEXT("/Game/Enemies/BP_Archer.BP_Archer_C")
	);
	UClass* Monster2 = LoadClass<AActor>(
		nullptr,
		TEXT("/Game/Enemies/BP_Tank.BP_Tank_C")
	);
	UClass* Monster3 = LoadClass<AActor>(
		nullptr,
		TEXT("/Game/Enemies/BP_TestEnemy.BP_TestEnemy_C")
	);
	TArray<UClass*> Monsters = { Monster1,Monster2,Monster3 };
	for (auto monsterPosition : monsterPositions) {
		int32 RandomIndex = FMath::RandRange(0, 2);
		GetWorld()->SpawnActor<ACharacter>(Monsters[RandomIndex], monsterPosition,FRotator());
	}
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

	// 如果没有传控制器，且DamageCauser是Pawn，就自动获取Controller
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
bool UEventCenter::UseTools(UClass* tool) {
	if (UWorld* world = GetWorld()) {
		if (tool == AToolHp::StaticClass()&&toolsNumber.hpTools > 0) {
			toolsNumber.hpTools -= 1;
			return true;
		}
		else if (tool == AToolMp::StaticClass() && toolsNumber.mpTools > 0) {
			toolsNumber.mpTools -= 1;
			return true;
		}
	}
	return false;
}

void UEventCenter::GetTools(AActor* tool, int toolNumber) {
	if (tool->IsA(AToolHp::StaticClass())) {
		toolsNumber.hpTools += 1;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 1.0f, FColor::Cyan,
				TEXT("Get A ToolHp"));
		}
	}
	else if(tool->IsA(AToolMp::StaticClass())){
		toolsNumber.mpTools += 1;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 1.0f, FColor::Cyan,
				TEXT("Get A ToolMp"));
		}
	}
	tool->Destroy();
}

void UEventCenter::SwitchToLevel() {

	if (UWorld* World = GetWorld())
	{
		FString PIEMapName = GetWorld()->GetMapName();
		FString RawMapName = PIEMapName;

		// 移除所有 UEDPIE_x_ 前缀（正则或字符串处理）
		if (RawMapName.StartsWith(TEXT("UEDPIE_")))
		{
			RawMapName = RawMapName.Mid(9);
		}

		// 拼接回原始路径
		FString CurrentLevelName = TEXT("/Game/ThirdPerson/Maps/") + RawMapName;

		if (CurrentLevelName.Equals(levelName, ESearchCase::IgnoreCase))
		{
			UE_LOG(LogTemp, Log, TEXT("Already in level '%s', updated spawn location."), *levelName);
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Diff! '%s'"), *levelName);
		UE_LOG(LogTemp, Log, TEXT("Diff! '%s'"), *CurrentLevelName);
		UGameplayStatics::OpenLevel(World, FName(*levelName));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SwitchToLevel: No valid world!"));
	}
}

const FVector UEventCenter::GetSpawnLocation() {
    return pawnLastLocation;
}
const FString UEventCenter::GetLevelName() {
	return levelName;
}
void UEventCenter::SetLevelAndLocation(FString name, FVector location) {
	levelName = name;
	pawnLastLocation = location;
}
void UEventCenter::WriteLastState() {
	// 1. 保存 PawnLastLocation (拆成 X/Y/Z)
	TSharedPtr<FJsonObject> RootObj = MakeShareable(new FJsonObject);

	TSharedPtr<FJsonObject> LocObj = MakeShareable(new FJsonObject());
	LocObj->SetNumberField("X", pawnLastLocation.X);
	LocObj->SetNumberField("Y", pawnLastLocation.Y);
	LocObj->SetNumberField("Z", pawnLastLocation.Z);
	RootObj->SetObjectField("PawnLastLocation", LocObj);

	// 2. 保存 LevelName
	RootObj->SetStringField("LevelName", levelName);

	// 3. 保存 ToolsNumber
	TSharedPtr<FJsonObject> ToolsObj = MakeShareable(new FJsonObject);
	ToolsObj->SetNumberField("hpTools", toolsNumber.hpTools);
	ToolsObj->SetNumberField("mpTools", toolsNumber.mpTools);
	RootObj->SetObjectField("ToolsNumber", ToolsObj);


	FString outputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(RootObj.ToSharedRef(), Writer);


	if (!FFileHelper::SaveStringToFile(outputString, *filePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save JSON to file: %s"), *filePath);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully Write lastState to: %s"), *filePath);
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
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonContent);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON in file: %s"), *filePath);
		return;
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

	UE_LOG(LogTemp, Log, TEXT("Successfully Read lastState from: %s"), *filePath);
}

void UEventCenter::ReadActorsPosition(TArray<FVector>& OutMonsterPositions,const FString& ActorFilePath)
{
	OutMonsterPositions.Empty();

	// 检查文件是否存在
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ActorFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Actor position file not found: %s"), *ActorFilePath);
		return;
	}

	// 读取文件内容
	FString JsonContent;
	if (!FFileHelper::LoadFileToString(JsonContent, *ActorFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to read actor position file: %s"), *ActorFilePath);
		return;
	}

	// 解析 JSON
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonContent);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON in actor file: %s"), *ActorFilePath);
		return;
	}

	if (TSharedPtr<FJsonValue> MonstersValue = JsonObject->TryGetField("Monsters"))
	{
		if (MonstersValue->Type == EJson::Array)
		{
			const TArray<TSharedPtr<FJsonValue>>& MonstersArray = MonstersValue->AsArray();

			for (const TSharedPtr<FJsonValue>& Element : MonstersArray)
			{
				if (Element.IsValid() && Element->Type == EJson::Object)
				{
					TSharedPtr<FJsonObject> LocObj = Element->AsObject();
					FVector Pos;
					Pos.X = LocObj->GetNumberField("X");
					Pos.Y = LocObj->GetNumberField("Y");
					Pos.Z = LocObj->GetNumberField("Z");
					OutMonsterPositions.Add(Pos);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("'Monsters' field exists but is not an array!"));
		}
	}

}