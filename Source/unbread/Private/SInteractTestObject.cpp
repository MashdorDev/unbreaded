// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractTestObject.h"

// Sets default values
ASInteractTestObject::ASInteractTestObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void ASInteractTestObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASInteractTestObject::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}



// Called every frame
void ASInteractTestObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

