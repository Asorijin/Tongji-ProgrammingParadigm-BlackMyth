// Fill out your copyright notice in the Description page of Project Settings.


#include "LandTemple.h"

// Sets default values
ALandTemple::ALandTemple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComponent"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CustomMeshFinder(
        TEXT("还没找到")  
    );

    if (CustomMeshFinder.Succeeded())
    {
        meshComponent->SetStaticMesh(CustomMeshFinder.Object);

        // 设置碰撞
        meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        meshComponent->SetCollisionObjectType(ECC_WorldStatic);
        meshComponent->SetCollisionResponseToAllChannels(ECR_Block);

        // 设置网格体缩放
        meshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    }
    else
    {
        // 回退到默认网格
        UE_LOG(LogTemp, Warning, TEXT("Custom mesh not found, using default cube"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshFinder(
            TEXT("/Engine/BasicShapes/Cube.Cube")
        );
        if (DefaultMeshFinder.Succeeded())
        {
            meshComponent->SetStaticMesh(DefaultMeshFinder.Object);
        }
    }
}

// Called when the game starts or when spawned
void ALandTemple::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALandTemple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALandTemple::SetLocation(FVector location) {
    SetActorLocation(location);
}

