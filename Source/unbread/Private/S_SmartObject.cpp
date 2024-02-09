// Fill out your copyright notice in the Description page of Project Settings.

#include "S_SmartObject.h"
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AS_SmartObject::AS_SmartObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	FacingDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Facing Direction"));
	FacingDirection->SetupAttachment(Billboard);
}

// Called when the game starts or when spawned
void AS_SmartObject::BeginPlay()
{
	Super::BeginPlay();
	
}



