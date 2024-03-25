// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SInteractTestObject.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNBREAD_API ASInteractTestObject : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASInteractTestObject();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BaseMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
