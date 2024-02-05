// Fill out your copyright notice in the Description page of Project Settings.


#include "SSpawnableBase.h"

// Sets default values
ASSpawnableBase::ASSpawnableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxTime = 1.0f;
	InitialVelocity = FVector();
	
}

// Called when the game starts or when spawned
void ASSpawnableBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ASSpawnableBase::Kill, MaxTime, false);
}

void ASSpawnableBase::Kill()
{
	// TODO object pooling
	this->Destroy();
}


