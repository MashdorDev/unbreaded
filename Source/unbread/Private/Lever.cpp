// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "ActivationInterface.h"

// Sets default values
ALever::ALever() : bIsActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Lever Mesh");
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALever::Interact_Implementation(AActor* InstigatingActor)
{
	IInteractInterface::Interact_Implementation(InstigatingActor);

	// Toggle bIsActive
	bIsActive = !bIsActive;

	if(bIsActive)
	{		
		// Set mesh state to be on
		// The actions taken here will depend on the mesh used

		// Call Activate interface function on all connected actors
		for(AActor* Actor : ConnectedActors)
		{
			if(Actor->GetClass()->ImplementsInterface(UActivationInterface::StaticClass()))
			{
				IActivationInterface::Execute_Activate(Actor);
			}
		}
	}

	else
	{
		// Set mesh state to be off
		// The actions taken here will depend on the mesh used

		// Call Deactivate interface function on all connected actors
		for(AActor* Actor : ConnectedActors)
		{
			if(Actor->GetClass()->ImplementsInterface(UActivationInterface::StaticClass()))
			{
				IActivationInterface::Execute_Deactivate(Actor);
			}
		}
	}
}

