// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_SmartObject.h"
#include "SPathActor.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API ASPathActor : public AS_SmartObject
{
	GENERATED_BODY()
public:
	ASPathActor();

	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplinePath")
	class USplineComponent* Path;

	void GetSplinePoints();

	TArray<FVector> Locations;
	
};
