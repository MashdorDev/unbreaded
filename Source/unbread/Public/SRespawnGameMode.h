// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SRespawnGameMode.generated.h"

/**
 * 
 */


UCLASS()

class UNBREAD_API ASRespawnGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurLives;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaxLives = 3;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnPlayer();

	UFUNCTION()
	void RespawnPlayer(AActor* Destroyed);

	UFUNCTION()
	void CheckLoss();

public:
	UFUNCTION(BlueprintCallable)
	void SetSpawnLocation(FTransform Location);
	
};
