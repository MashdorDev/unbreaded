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

	// Temp code for testing pause game
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
	UnusedHandle, this, &ASRespawnGameMode::PauseGame, 2.0f, false);
	
	
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
		EndGame(false);
		return true;
	}
	return false;
}

void ASRespawnGameMode::EndGame(bool Won)
{
	GameOver.Broadcast(Won);
}

void ASRespawnGameMode::SetSpawnLocation(FTransform Location)
{
	SpawnLocation = Location;
}

void ASRespawnGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GamePaused.Broadcast(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Paused"));

	
}

void ASRespawnGameMode::UnpauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GamePaused.Broadcast(false);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unpaused"));


}
