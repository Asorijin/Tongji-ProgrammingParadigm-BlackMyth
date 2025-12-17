// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterConfig.h"
#include "Json.h"
#include "Misc/FileHelper.h"
UCharacterConfig::UCharacterConfig() {
	
}
void UCharacterConfig::Initialize() {

    FString jsonString;

    TSharedPtr<FJsonObject> jsonObject;

    if (!FFileHelper::LoadFileToString(jsonString, *filePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *filePath);
        return;
    }

    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(jsonString);

    if (!FJsonSerializer::Deserialize(JsonReader, jsonObject))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON: %s"), *jsonString);
        return;
    }

    _maxHp = jsonObject->GetNumberField(TEXT("_maxHp"));
    _maxMp = jsonObject->GetNumberField(TEXT("_maxMp"));
    _hp = jsonObject->GetNumberField(TEXT("_hp"));
    _mp = jsonObject->GetNumberField(TEXT("_mp"));
    _attack = jsonObject->GetNumberField(TEXT("_attack"));
    _defence = jsonObject->GetNumberField(TEXT("_defence"));
}
void UCharacterConfig::WriteConfigData() {

    TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);

    jsonObject->SetNumberField(TEXT("_maxHp"), _maxHp);
    jsonObject->SetNumberField(TEXT("_maxMp"), _maxMp);
    jsonObject->SetNumberField(TEXT("_hp"), _hp);
    jsonObject->SetNumberField(TEXT("_mp"), _mp);  
    jsonObject->SetNumberField(TEXT("_attack"), _attack);
    jsonObject->SetNumberField(TEXT("_defence"), _defence);

    FString outputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&outputString);
    FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);

    if (!FFileHelper::SaveStringToFile(outputString, *filePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save JSON to file: %s"), *filePath);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully saved character config to: %s"), *filePath);
    }
}
