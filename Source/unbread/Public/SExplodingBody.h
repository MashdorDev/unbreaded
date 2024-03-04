// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SExplodingBody.generated.h"

class ASCrumbles;
class UCapsuleComponent;
class URadialForceComponent;

UCLASS()
class UNBREAD_API ASExplodingBody : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASExplodingBody();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	URadialForceComponent* RadialForce;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASCrumbles> CrumblesActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Explode();

};
