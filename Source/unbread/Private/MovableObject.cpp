// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"

// Sets default values
AMovableObject::AMovableObject() : bIsBeingMoved(false), MaxInteractionDistance(10.0f), bCanMove(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
}

// Called when the game starts or when spawned
void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableObject::Interact_Implementation(AActor* InstigatingActor)
{
	IInteractInterface::Interact_Implementation(InstigatingActor);

	if(!bCanMove)
	{
		return;
	}

	// Do nothing if the instigating actor is too far away
	if(FVector::Distance(GetTransform().GetLocation(), InstigatingActor->GetTransform().GetLocation()) > MaxInteractionDistance)
	{
		return;
	}
	
	bIsBeingMoved = !bIsBeingMoved;

	if(bIsBeingMoved)
	{		
		AttachToActor(InstigatingActor, FAttachmentTransformRules::KeepWorldTransform);
	}

	else
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

