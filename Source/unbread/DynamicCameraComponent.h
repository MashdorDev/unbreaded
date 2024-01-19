// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/DynamicCameraInterface.h"
#include "DynamicCameraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNBREAD_API UDynamicCameraComponent : public UActorComponent, public IDynamicCameraInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDynamicCameraComponent();

	// A reference to the actor containing the default camera (This will likely be the actor this component is attached to)
	UPROPERTY(EditAnywhere)
	AActor* DefaultCameraActor;

	// A reference to the actor containing the next camera that will be transitioned to during gameplay
	UPROPERTY(BlueprintReadWrite)
	AActor* NextCameraActor;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetNextCamera_Implementation(AActor* CameraActor) override;

	void TransitionCamera_Implementation(const float TransitionTime) override;

		
};
