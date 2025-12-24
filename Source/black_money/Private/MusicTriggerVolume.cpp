#include "MusicTriggerVolume.h"
#include "Components/BoxComponent.h"
#include "black_money/black_moneyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMusicTriggerVolume::AMusicTriggerVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMusicTriggerVolume::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMusicTriggerVolume::OnOverlapEnd);

    RootComponent = TriggerBox;

    CurrentMusicComponent = nullptr;
}

void AMusicTriggerVolume::BeginPlay()
{
    AActor::BeginPlay();
}

void AMusicTriggerVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bOnlyAffectPlayer)
    {
        if (!OtherActor || !OtherActor->IsA(Ablack_moneyCharacter::StaticClass()))
            return;
    }

    if (BackgroundMusic && GetWorld())
    {
        if (CurrentMusicComponent && CurrentMusicComponent->IsPlaying())
        {
            CurrentMusicComponent->Stop();
        }

        // 播放新音乐（2D，全局背景音）
        CurrentMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BackgroundMusic);
        if (CurrentMusicComponent)
        {
            CurrentMusicComponent->bAllowSpatialization = false; // 确保是 2D 背景音乐
            CurrentMusicComponent->bAutoDestroy = true;          // 播放完自动销毁
            CurrentMusicComponent->bOverrideAttenuation = true;
            CurrentMusicComponent->AttenuationSettings = nullptr; // 无衰减
        }
    }
}

void AMusicTriggerVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (bOnlyAffectPlayer)
    {
        if (!OtherActor || !OtherActor->IsA(Ablack_moneyCharacter::StaticClass()))
            return;
    }

    if (DefaultMusic && GetWorld())
    {
        if (CurrentMusicComponent && CurrentMusicComponent->IsPlaying())
        {
            CurrentMusicComponent->Stop();
        }

        // 播放默认音乐（2D，全局背景音）
        CurrentMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), DefaultMusic);
        if (CurrentMusicComponent)
        {
            CurrentMusicComponent->bAllowSpatialization = false; // 确保是 2D 背景音乐
            CurrentMusicComponent->bAutoDestroy = true;          // 播放完自动销毁
            CurrentMusicComponent->bOverrideAttenuation = true;
            CurrentMusicComponent->AttenuationSettings = nullptr; // 无衰减
        }
    }
}