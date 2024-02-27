// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplodingBody.h"

#include "SCrumbles.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplodingBody::ASExplodingBody()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(Root);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForce->SetupAttachment(Root);
	RadialForce->Radius = 200.f;
	RadialForce->ImpulseStrength = 1000.f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	Capsule->SetupAttachment(Root);
	Capsule->SetRelativeLocation(FVector(0.f, 0.f, 88.f));
	Capsule->SetCapsuleHalfHeight(88.f);
	Capsule->SetCapsuleRadius(34.f);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	Mesh->SetupAttachment(Capsule);
	Mesh->SetRelativeScale3D(FVector(0.465f, 0.465f, 0.465f));
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

}

// Called when the game starts or when spawned
void ASExplodingBody::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASExplodingBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASExplodingBody::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASExplodingBody::Explode()
{
	RadialForce->FireImpulse();
	const FActorSpawnParameters SpawnParameters;
	GetWorld()->SpawnActor<ASCrumbles>(CrumblesActor, Mesh->GetComponentLocation(), Mesh->GetComponentRotation(), SpawnParameters);
	Destroy();
}

