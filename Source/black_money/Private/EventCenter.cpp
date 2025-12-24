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

// 鍓嶅悜澹版槑锛岄伩鍏嶅惊鐜緷璧?
class Ablack_moneyCharacter;

UEventCenter::UEventCenter() {
	
}

void UEventCenter::GenerateActors() {

	TArray<FVector> monsterPositions, templeLandPositions;

	FString nowPath = ActorsfilePath / FPaths::GetBaseFilename(levelName) / TEXT("ActororPosition.txt");

	ReadActorsPosition(monsterPositions, templeLandPositions,nowPath);

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

	// 濡傛灉娌′紶鎺у埗鍣紝鑰?DamageCauser 鏄?Pawn锛屽氨鑷姩鍙栧叾 Controller
	if (!EventInstigator && DamageCauser)
	{
		if (APawn* PawnCauser = Cast<APawn>(DamageCauser))
		{
			EventInstigator = PawnCauser->GetController();
		}
	}

	
	FDamageEvent DamageEvent;
	// 璁剧疆浼ゅ绫诲瀷
	DamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());

	// 璋冪敤 Actor 鐨?TakeDamage
	const float ActualDamage = DamagedActor->TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser);

	// 杈撳嚭鏃ュ織
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
		// 鍏朵粬绫诲瀷宸ュ叿鐨勫鐞?
	}
	tool->Destroy();
}

void UEventCenter::SwitchToLevel() {

	if (UWorld* World = GetWorld())
	{
		// 鑾峰彇褰撳墠鍏冲崱鐨勭煭鍚嶇О
		FString CurrentLevelName = GetWorld()->GetMapName();

		// 姣旇緝鐩爣鍏冲崱鍚嶅拰褰撳墠鍏冲崱鍚?
		if (CurrentLevelName.Equals(levelName, ESearchCase::IgnoreCase))
		{
			UE_LOG(LogTemp, Log, TEXT("Already in level '%s', updated spawn location."), *levelName);
			return;
		}

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

	UE_LOG(LogTemp, Log, TEXT("Successfully loaded save data from: %s"), *filePath);
}

void UEventCenter::ReadActorsPosition(TArray<FVector>& OutMonsterPositions, TArray<FVector>& OutTempleLandPositions, const FString& ActorFilePath)
{
	// 清空输出数组（可选，根据需求）
	OutMonsterPositions.Empty();
	OutTempleLandPositions.Empty();

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

	if (TSharedPtr<FJsonValue> TempleLandsValue = JsonObject->TryGetField("TempleLands"))
	{
		if (TempleLandsValue->Type == EJson::Array)
		{
			const TArray<TSharedPtr<FJsonValue>>& TempleLandsArray = TempleLandsValue->AsArray();
			for (const TSharedPtr<FJsonValue>& Element : TempleLandsArray)
			{
				if (Element.IsValid() && Element->Type == EJson::Object)
				{
					TSharedPtr<FJsonObject> LocObj = Element->AsObject();
					FVector Pos;
					Pos.X = LocObj->GetNumberField("X");
					Pos.Y = LocObj->GetNumberField("Y");
					Pos.Z = LocObj->GetNumberField("Z");
					OutTempleLandPositions.Add(Pos);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("'TempleLands' field is not an array!"));
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Loaded %d monsters and %d temple lands from %s"),
		OutMonsterPositions.Num(), OutTempleLandPositions.Num(), *ActorFilePath);
}