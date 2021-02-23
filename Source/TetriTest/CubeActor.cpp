// Fill out your copyright notice in the Description page of Project Settings.



#include "CubeActor.h"
#include "CubeComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Figure.h"

// Sets default values
ACubeActor::ACubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box")); //init
	//set variables, in our case we making box visible in game
	//BoxComp->bHiddenInGame = false;
	//set component as root
	RootComponent = BoxComp; 
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/Cube.Cube'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BoxComp->SetStaticMesh(Asset);
	int type = rand() % 3;
	
	GunMode mode = IntToMode(type);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>MeshMaterialMove(TEXT("Material'/Game/FirstPerson/Meshes/CubeMaterialMove.CubeMaterialMove'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>MeshMaterialRotate(TEXT("Material'/Game/FirstPerson/Meshes/CubeMaterialRotate.CubeMaterialRotate'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>MeshMaterialDestroy(TEXT("Material'/Game/FirstPerson/Meshes/CubeMaterialDestroy.CubeMaterialDestroy'"));

	UMaterialInterface* material = nullptr;

	switch (mode) {
	case GunMode::default:
	case GunMode::pull:
	case GunMode::push: if (MeshMaterialMove.Succeeded()) { material = MeshMaterialMove.Object; } break;
	case GunMode::rotateCounter:
	case GunMode::rotate: if (MeshMaterialRotate.Succeeded()) { material = MeshMaterialRotate.Object; } break;
	case GunMode::destroyFigure:
	case GunMode::destroy: if (MeshMaterialDestroy.Succeeded()) { material = MeshMaterialDestroy.Object; } break;
	}

	if (material != nullptr) {
		BoxComp->SetMaterial(0, material);
	}
	
	BoxComp->CanCharacterStepUpOn = ECB_Yes;

	CubeComp = CreateDefaultSubobject<UCubeComponent>(TEXT("Cube"));
	CubeComp->currentMode = mode;

	AddOwnedComponent(CubeComp); 

#define scale 250.f
	BoxComp->SetRelativeScale3D(FVector(scale, scale, scale));

}