// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "MovableObject.generated.h"

UCLASS()
class UNBREAD_API AMovableObject : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableObject();

	// Components

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	// Properties

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	bool bIsBeingMoved;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	bool bCanMove;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Setup")
	float MaxInteractionDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(AActor* InstigatingActor) override;
};
