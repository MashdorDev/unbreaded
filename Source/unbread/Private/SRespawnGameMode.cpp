// Fill out your copyright notice in the Description page of Project Settings.


#include "SRespawnGameMode.h"

#include "Chaos/SpatialAccelerationCollection.h"
#include "GameFramework/DefaultPawn.h"


void ASRespawnGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	CurLives = MaxLives;

	SetSpawnLocation(FindPlayerStart(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetTransform());
	UGameplayStatics::GetPlayerPawn(GetWorld(),0)->OnDestroyed.AddDynamic(this, &ASRespawnGameMode::RespawnPlayer);
	
}

void ASRespawnGameMode::SpawnPlayer()
{
	// spawn and possess player
	if(APawn* SpawnedPlayer = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation))
	{
		SpawnedPlayer->OnDestroyed.AddDynamic(this, &ASRespawnGameMode::RespawnPlayer);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(SpawnedPlayer);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could not spawn player"));
	}
}

void ASRespawnGameMode::RespawnPlayer(AActor* Destroyed)
{
	CurLives--;

	if (CheckLoss()) return;
	
	// spawn player with delay, or no delay if 0.
	if(SpawnDelay > 0.0f)
	{
		FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
        UnusedHandle, this, &ASRespawnGameMode::SpawnPlayer, SpawnDelay, false);
	}
	else SpawnPlayer();
	
}

bool ASRespawnGameMode::CheckLoss()
{
	if(CurLives <= 0)
	{
		EndGame();
		return true;
	}
	return false;
}

void ASRespawnGameMode::EndGame()
{
	GameOver.Broadcast(false);
}

void ASRespawnGameMode::SetSpawnLocation(FTransform Location)
{
	SpawnLocation = Location;
}
