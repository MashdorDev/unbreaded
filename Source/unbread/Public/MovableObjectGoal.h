// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableObjectGoal.generated.h"

class AMovableObject;
class UBoxComponent;

UCLASS()
class UNBREAD_API AMovableObjectGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableObjectGoal();

	// Components

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Components")
	UBoxComponent* GoalTrigger;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* GuideMesh;

	// Properties

	// The object that must be moved to this goal
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Setup")
	AMovableObject* KeyObject;

	// An array of objects that will be activated once the key has been brought to this goal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TArray<AActor*> ConnectedActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnEnterVolume(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
