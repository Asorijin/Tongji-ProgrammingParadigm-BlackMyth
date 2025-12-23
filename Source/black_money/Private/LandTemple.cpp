// Fill out your copyright notice in the Description page of Project Settings.


#include "LandTemple.h"
#include "black_money/black_moneyCharacter.h"
// Sets default values
ALandTemple::ALandTemple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    

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
            TEXT("/Engine/BasicShapes/Sphere.Sphere")
        );
        if (DefaultMeshFinder.Succeeded())
        {
            meshComponent->SetStaticMesh(DefaultMeshFinder.Object);
        }
    }

    RootComponent = meshComponent;

    // 创建交互触发器（球形）
    InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionTrigger"));
    InteractionTrigger->SetupAttachment(RootComponent);
    InteractionTrigger->SetSphereRadius(200.0f);
    InteractionTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


    // 绑定重叠事件（必须在构造函数中绑定！）
    InteractionTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALandTemple::OnOverlapBegin);
    InteractionTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &ALandTemple::OnOverlapEnd);



    // 创建 WidgetComponent（必须在构造函数中创建）
    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootComponent);
    InteractionWidget->SetWidgetSpace(EWidgetSpace::World); // 世界空间
    InteractionWidget->SetDrawSize(FVector2D(200.0f, 50.0f));
    InteractionWidget->SetPivot(FVector2D(0.5f, 0.0f)); // 底部居中对齐
    InteractionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f)); // 在物体上方 100 单位
    InteractionWidget->SetVisibility(false); // 初始隐藏

}

// Called when the game starts or when spawned
void ALandTemple::BeginPlay()
{
	AActor::BeginPlay();
	
    CachedPlayerController = GetWorld()->GetFirstPlayerController();

    Tags.Add(FName("LandTemple"));

    if (UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr,
        TEXT("/Game/BP/SceneObjects/InteractionPrompt.InteractionPrompt_C")))
    {
        InteractionWidget->SetWidgetClass(WidgetClass);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load WBP_InteractionPrompt_C!"));
    }
}

// Called every frame
void ALandTemple::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
    if (CachedPlayerController != nullptr) {
        CachedPlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

        WidgetLocation = InteractionWidget->GetComponentLocation();

        LookAtDirection = CameraLocation - WidgetLocation;
        LookAtDirection.Z = 0.0f; //锁定水平面
        LookAtDirection.Normalize();

        if (!LookAtDirection.IsNearlyZero())
        {
            FRotationMatrix RotMatrix(LookAtDirection.Rotation());
            FQuat WorldRot = RotMatrix.ToQuat();

            FTransform NewTransform(WorldRot, WidgetLocation);
            InteractionWidget->SetWorldTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
        }
    }
}

void ALandTemple::SetLocation(FVector location) {
    SetActorLocation(location);
}

void ALandTemple::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 检查是否是玩家角色（Pawn）
    if (OtherActor && OtherActor != this)
    {
        if (OtherActor->IsA(Ablack_moneyCharacter::StaticClass()))
        {
            InteractionWidget->SetVisibility(true);

            Cast<Ablack_moneyCharacter>(OtherActor)->nearbyInteraction.Add(this);
        }
    }
}

void ALandTemple::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        if (Cast<Ablack_moneyCharacter>(OtherActor))
        {
            InteractionWidget->SetVisibility(false);

            if(OtherActor && Cast<Ablack_moneyCharacter>(OtherActor)->nearbyInteraction.Contains(this))
                Cast<Ablack_moneyCharacter>(OtherActor)->nearbyInteraction.Remove(this);
        }
    }
}

