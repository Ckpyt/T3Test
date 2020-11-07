// Fill out your copyright notice in the Description page of Project Settings.



#include "CubeActor.h"
#include "CubeComponent.h"


// Sets default values
ACubeActor::ACubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box")); //init
	//set varables, in our case we making box visible in game
	//BoxComp->bHiddenInGame = false;
	//set component as root
	RootComponent = BoxComp; //CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/Cube.Cube'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BoxComp->SetStaticMesh(Asset);
	int type = rand() % 3;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>MeshMaterialMove(TEXT("Material'/Game/FirstPerson/Meshes/CubeMaterialMove.CubeMaterialMove'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>MeshMaterialRotate(TEXT("Material'/Game/FirstPerson/Meshes/CubeMaterialRotate.CubeMaterialRotate'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>MeshMaterialDestroy(TEXT("Material'/Game/FirstPerson/Meshes/CubeMaterialDestroy.CubeMaterialDestroy'"));

	UMaterialInterface* material = nullptr;

	switch (type) {
	case 0: if (MeshMaterialMove.Succeeded()) { material = MeshMaterialMove.Object; } break;
	case 1: if (MeshMaterialRotate.Succeeded()) { material = MeshMaterialRotate.Object; } break;
	case 2: if (MeshMaterialDestroy.Succeeded()) { material = MeshMaterialDestroy.Object; } break;
	}

	if (material != nullptr) {
		BoxComp->SetMaterial(0, material);
	}

	//BoxComp->BodyInstance.SetCollisionProfileName("Projectile");
	//BoxComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//BoxComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Default, 0.f));
	BoxComp->CanCharacterStepUpOn = ECB_Yes;

	CubeComp = CreateDefaultSubobject<UCubeComponent>(TEXT("Cube"));
	CubeComp->currentMode = type;

	AddOwnedComponent(CubeComp);

#define scale 250.f
	BoxComp->SetRelativeScale3D(FVector(scale, scale, scale));
}

// Called when the game starts or when spawned
void ACubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

