// Fill out your copyright notice in the Description page of Project Settings.


#include "SRespawnGameMode.h"

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
	// converting to Fvector and Frotator
	const FVector Pos = SpawnLocation.GetLocation();
	const FRotator* Rot = new FRotator(SpawnLocation.GetRotation().Rotator());


	// spawn actor and bind on destroy
	UClass* PawnClass = DefaultPawnClass;
	APawn* SpawnedPlayer = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnLocation);
	SpawnedPlayer->OnDestroyed.AddDynamic(this, &ASRespawnGameMode::RespawnPlayer);

	// possess spawned player
	if(APawn* SpawnedPawn = dynamic_cast<APawn*>(SpawnedPlayer))
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(SpawnedPawn);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Possessed player"));

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cannot cast AActor to APawn"));
	}
}

void ASRespawnGameMode::RespawnPlayer(AActor* Destroyed)
{
	CurLives--;
	CheckLoss();
	
	// spawn player with delay, or no delay if 0.
	if(SpawnDelay > 0.0f)
	{
		FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
        UnusedHandle, this, &ASRespawnGameMode::SpawnPlayer, SpawnDelay, false);
	}
	else SpawnPlayer();
	
}

void ASRespawnGameMode::CheckLoss()
{
	if(CurLives <= 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

void ASRespawnGameMode::SetSpawnLocation(FTransform Location)
{
	SpawnLocation = Location;
}
