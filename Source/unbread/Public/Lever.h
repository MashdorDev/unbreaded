// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

UCLASS()
class UNBREAD_API ALever : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALever();

	// Components

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	// Properties
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ConnectedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interaction interface functions
	void Interact_Implementation(AActor* InstigatingActor) override;

};
