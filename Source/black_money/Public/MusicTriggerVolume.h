// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MusicTriggerVolume.generated.h"

UCLASS()
class AMusicTriggerVolume : public AActor
{
    GENERATED_BODY()

public:
    AMusicTriggerVolume();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    // 可在编辑器中指定要播放的背景音乐
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundBase* BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundBase* DefaultMusic;

    // 是否只对玩家生效
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
    bool bOnlyAffectPlayer = true;

private:
    UBoxComponent* TriggerBox;
    UAudioComponent* CurrentMusicComponent;
};