// Fill out your copyright notice in the Description page of Project Settings.


#include "S_FallingKnives.h"

#include "SCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AS_FallingKnives::AS_FallingKnives()
{

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	MinSpawnDelay = 0.5f;
	MaxSpawnDelay = 1.0f;
	isActive = false;
	RandomSpawnDelay = 0.0f;
	SpawnLocation = FVector();
	TriggerArea = nullptr;

}

// Called when the game starts or when spawned
void AS_FallingKnives::BeginPlay()
{
	Super::BeginPlay();
	GetBoxComponent();
	
}

void AS_FallingKnives::SpawnObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("spawn knife"));

	InstantiateObject();
	InstantiateObject();
	InstantiateObject();

	
	// call back if still active
	if(isActive)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
			FString::Printf(TEXT("height: %f"), SpawnHeight));	
		RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AS_FallingKnives::SpawnObject, RandomSpawnDelay, false);
	}
}

void AS_FallingKnives::InstantiateObject()
{
	// generate random location within collider
	FVector Origin = TriggerArea->Bounds.Origin;
	FVector Extent = TriggerArea->Bounds.BoxExtent;
	SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(FVector(Origin.X, Origin.Y, Origin.Z + Extent.Z + SpawnHeight),
		FVector(Extent.X, Extent.Y, 0.0f));

	// spawn actor
	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, FRotator(0.0f), SpawnParams);
}


void AS_FallingKnives::GetBoxComponent()
{
	// set trigger area to the BoxComponent on the actor
	TArray<UBoxComponent*> Colliders;
	GetComponents<UBoxComponent>(Colliders);
	
	if(Colliders.Num() > 0)
	{
		// bind custom overlap functions
		TriggerArea = *Colliders.GetData();
		TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AS_FallingKnives::OnEnterArea);
		TriggerArea->OnComponentEndOverlap.AddDynamic(this, &AS_FallingKnives::OnExitArea);
		
	}
	else GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No boxcomponent on actor"));	
}

void AS_FallingKnives::BeginTick()
{
	this->SetActorTickEnabled(true);
}


void AS_FallingKnives::OnEnterArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ASCharacter>(OtherActor))
	{
		isActive = true;
		RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AS_FallingKnives::SpawnObject, RandomSpawnDelay, false);
	}
	
	
}

void AS_FallingKnives::OnExitArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// TODO: change this to some sort of interface comparison rather than cast
	if(Cast<ASCharacter>(OtherActor))
	{
		isActive = false;
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
			
	}
}


// Called every frame
void AS_FallingKnives::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// for some reason this actor wont tick TODO
}

