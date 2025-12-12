// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterConfig.h"
#include "Json.h"
#include "Misc/FileHelper.h"
//获取到该类后，先在Character中Read，再去使用数据
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

    _maxHp = jsonObject->GetIntegerField(TEXT("_maxHp"));
    _maxMp = jsonObject->GetIntegerField(TEXT("_maxMp"));
    _hp = jsonObject->GetIntegerField(TEXT("_hp"));
    _mp = jsonObject->GetIntegerField(TEXT("_mp"));
    _attack = jsonObject->GetIntegerField(TEXT("_attack"));
    _defence = jsonObject->GetIntegerField(TEXT("_defence"));
    _speed = jsonObject->GetIntegerField(TEXT("_speed"));
    _equipment._head = jsonObject->GetIntegerField(TEXT("_equipment._head"));
    _equipment._body = jsonObject->GetIntegerField(TEXT("_equipment._body"));
    _equipment._boot = jsonObject->GetIntegerField(TEXT("_equipment._boot"));
    _equipment._weapon = jsonObject->GetIntegerField(TEXT("_equipment._weapon"));
    _maxHp = jsonObject->GetIntegerField(TEXT("_maxHp"));
}
void UCharacterConfig::WriteConfigData() {

    TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);

    jsonObject->SetNumberField(TEXT("_maxHp"), _maxHp);
    jsonObject->SetNumberField(TEXT("_maxMp"), _maxMp);
    jsonObject->SetNumberField(TEXT("_hp"), _hp);
    jsonObject->SetNumberField(TEXT("_mp"), _mp);  
    jsonObject->SetNumberField(TEXT("_attack"), _attack);
    jsonObject->SetNumberField(TEXT("_defence"), _defence);
    jsonObject->SetNumberField(TEXT("_speed"), _speed);

    TSharedPtr<FJsonObject> equipmentObject = MakeShareable(new FJsonObject);
    equipmentObject->SetNumberField(TEXT("_head"), _equipment._head);
    equipmentObject->SetNumberField(TEXT("_body"), _equipment._body);
    equipmentObject->SetNumberField(TEXT("_boot"), _equipment._boot);
    equipmentObject->SetNumberField(TEXT("_weapon"), _equipment._weapon);

    jsonObject->SetObjectField(TEXT("_equipment"), equipmentObject);

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
