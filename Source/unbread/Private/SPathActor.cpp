// Fill out your copyright notice in the Description page of Project Settings.


#include "SPathActor.h"

#include "Components/SplineComponent.h"

ASPathActor::ASPathActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("PATH"));
	Path->SetupAttachment(RootComponent);
	Path->bDrawDebug = true;
}

void ASPathActor::BeginPlay()
{
	Super::BeginPlay();
	GetSplinePoints();
}

void ASPathActor::GetSplinePoints()
{
	for(int i = 0; i < Path->GetNumberOfSplinePoints(); ++i)
	{
		Locations.Add(FLocation{Path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World), false});
	}
}
