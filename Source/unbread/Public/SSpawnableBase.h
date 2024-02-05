// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSpawnableBase.generated.h"

UCLASS()
class UNBREAD_API ASSpawnableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASSpawnableBase();

	UPROPERTY(EditAnywhere)
	FVector InitialVelocity;

	UPROPERTY(EditAnywhere)
	float MaxTime;

	UFUNCTION()
	void Kill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DeathTimer;



};
