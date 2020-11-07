// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeComponent.h"
#include "CubeActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "TetriTestProjectile.h"
#include "TetriTestGameMode.h"


const float UCubeComponent::size = 1000.0f;

// Sets default values
UCubeComponent::UCubeComponent()
{
	
	LStream Stream;
	std::cout.rdbuf(&Stream);

	//std::cout << "do it! 3" << std::endl;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	TArray<UStaticMeshComponent*> Components;
	auto owner = GetOwner();
	if (owner != nullptr) {
		owner->GetComponents<UStaticMeshComponent>(Components, true);
		if (Components.Num() > 0) {

			//std::cout << "do it components started" << std::endl;

			cube = Components[0];
			CollisionComp = Components[0];

			//cube->SetWorldScale3D(FVector(10.0f, 10.0f, 10.0f));

			//RootComponent = cube;

			// Box trigger / Collider
			//CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
			//CollisionComp->bGenerateOverlapEvents = true;
			//CollisionComp->SetWorldScale3D(FVector(10.0f, 10.0f, 10.0f));
			//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UCubeComponent::OnOverlapBegin);
			//CollisionComp->OnComponentEndOverlap.AddDynamic(this, &UCubeComponent::OnOverlapEnd);
			//CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
			CollisionComp->OnComponentHit.AddDynamic(this, &UCubeComponent::OnHit);		// set up a notification for when this component hits something blocking

			// Use a ProjectileMovementComponent to govern this projectile's movement
			/*ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
			ProjectileMovement->UpdatedComponent = cube;
			ProjectileMovement->InitialSpeed = 10.f;
			ProjectileMovement->MaxSpeed = 10.f;
			ProjectileMovement->bRotationFollowsVelocity = true;
			ProjectileMovement->bShouldBounce = true;
			ProjectileMovement->Activate(true); */
			//std::cout << "do it finished" << std::endl;
		}
	}
}

// Called when the game starts or when spawned
void UCubeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
// Called every frame
void ACubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


int UCubeComponent::CalcCrossedSide(FVector other) {
	FVector pos = cube->GetComponentLocation();
	

	//std::cout << other.Z << " " << pos.Z << " " << cube->GetComponentLocation().Z << std::endl;

	FVector impulse = other - pos;
	float x = impulse.X > 0 ? impulse.X : -impulse.X;
	float y = impulse.Y > 0 ? impulse.Y : -impulse.Y;
	float z = impulse.Z > 0 ? impulse.Z : -impulse.Z;

	//std::cout << "x" << x << "y" << y << "z" << z << std::endl;

	if (x > 520.0f) {
		return impulse.X > 0 ? 1 : -1;
	}
	else if (y > 520.0f) {
		return impulse.Y > 0 ? 2 : -2;
	}
	else {
		return impulse.Z > 0 ? 3 : -3;
	}

	return 0;
}

void UCubeComponent::AddChild(UCubeComponent* child) { figure.push_back(child); }

void UCubeComponent::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
// 	LStream Stream;
// 	std::cout.rdbuf(&Stream);
	//std::cout << "Hit! " << std::endl;

	if ((OtherActor != NULL) && /*(OtherActor != this) &&*/ (OtherComp != NULL) /*&& OtherComp->IsSimulatingPhysics()*/)
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		ATetriTestProjectile* bullet = dynamic_cast<ATetriTestProjectile*>(OtherActor);
		if (bullet != nullptr) {
			int mode = bullet->mode;
			int side = CalcCrossedSide(OtherActor->GetActorLocation());
/*			std::cout << "Hit! " << side << std::endl;*/
			switch (mode) {
			case 0: Push(side); break;
			case 3: Pull(side); break;

			case 1: Rotate(side); break;
			case 4: CouterRotate(side); break;

			case 2: Destroy(); break;
			case 5: DestroyFigure(); break;
			}
		}
	}
}

void UCubeComponent::Init(mode curMode) {currentMode = (int)curMode;}

void UCubeComponent::Push(int side, bool push) {
// 	LStream Stream;
// 	std::cout.rdbuf(&Stream);
// 	std::cout << "Push" << side << std::endl;
	if (!push) {
		if (parent != nullptr)
			return parent->Push(side);
		//else
			//for (auto cb : figure)
				//cb->Push(side, true);
	}
	
	auto pos = this->GetOwner()->GetActorLocation();

	switch (side) {
	case 1:
	case -1: pos.X -= side * size; break;
	case 2: pos.Y -= size; break;
	case -2: pos.Y += size; break;
	case 3: pos.Z -= size; break;
	case -3: pos.Z += size; break;
	default: break;
	}
	if(ATetriTestGameMode::GetGameMode()->CheckMoveBlock(pos))
		this->GetOwner()->SetActorLocation(pos);
}

void UCubeComponent::Pull(int side) {
	Push(-side);
}

void UCubeComponent::Rotate(int side) {
	if (parent != nullptr)
		return parent->Rotate(side);
}


void UCubeComponent::CouterRotate(int side) {
	Rotate(-side);
}

void UCubeComponent::DestroyFigure() {

	if (parent == nullptr || figure.size() > 1) {
		for (auto var : figure)
		{
			if (var != nullptr && var != this)
				var->GetOwner()->Destroy();
		}
		
		GetOwner()->Destroy();
	}
	else {
		parent->DestroyFigure();
	}
}

void UCubeComponent::Destroy() {
	//move figure holder to another block
	if (parent == nullptr && figure.size() > 0) {
		auto newparent = figure[0];
		newparent->parent = nullptr;
		for (int i = 1; i < figure.size(); i++) {
			newparent->figure.push_back(figure[i]);
			figure[i]->parent = newparent;
		}
	}

	switch (currentMode) {
	case 0: ATetriTestCharacter::AddPushCharges(); break;
	case 1: ATetriTestCharacter::AddRotateCharges(); break;
	case 2: ATetriTestCharacter::AddDestroyCharges(); break;
	}
	GetOwner()->Destroy();
}

UCubeComponent* UCubeComponent::SpawnBlock(int x, int y, UCubeComponent* owner, UWorld* const World) {

	static const TSubclassOf<class ACubeActor> ProjectileClass;
	if (World != NULL)
	{

		FRotator SpawnRotation(0.f,0.f,0.f);
		FVector SpawnLocation(((float)x - 1.5f) * 1000.f, ((float)y - 1.5f) * 1000.f, 19000.f);
		FActorSpawnParameters ActorSpawnParams;

		//Set Spawn Collision Handling Override
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		ACubeActor* actor = World->SpawnActor<ACubeActor>(ACubeActor::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams); //mode and material in the constructor
		if (actor != nullptr) {
			UCubeComponent* cube = actor->CubeComp;
			if (cube != nullptr) {
				if (owner == nullptr) {
					owner = cube;
				}
				else {
					owner->figure.push_back(cube);
					cube->parent = owner;
				}
			}
		}
	}

	return owner;
}