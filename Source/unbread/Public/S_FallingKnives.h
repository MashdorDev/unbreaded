// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "S_FallingKnives.generated.h"

class UBoxComponent;
UCLASS()
class UNBREAD_API AS_FallingKnives : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS_FallingKnives();

	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerArea;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere)
	float SpawnHeight;
	

	UFUNCTION()
	void GetBoxComponent();

	UFUNCTION()
	void BeginTick();
	
	UFUNCTION()
	void OnEnterArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExitArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnObject();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpawnDelay;

	FTimerHandle SpawnTimer;
	float RandomSpawnDelay;
	bool isActive;
	FVector SpawnLocation;
	

};
