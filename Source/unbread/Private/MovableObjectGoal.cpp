// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObjectGoal.h"

#include "MovableObject.h"
#include "Components/BoxComponent.h"

// Sets default values
AMovableObjectGoal::AMovableObjectGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up component attachments
	GoalTrigger = CreateDefaultSubobject<UBoxComponent>("Goal Trigger");
	SetRootComponent(GoalTrigger);

	GuideMesh = CreateDefaultSubobject<UStaticMeshComponent>("Guide Mesh");
	GuideMesh->SetupAttachment(GoalTrigger);

	// Set the goal trigger call OnEnterVolume
	GoalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMovableObjectGoal::OnEnterVolume);

}

// Called when the game starts or when spawned
void AMovableObjectGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovableObjectGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableObjectGoal::OnEnterVolume(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Do nothing if the other actor is not linked
	if(OtherActor != Cast<AActor>(LinkedObject))
	{
		return;
	}

	// Detach the linked object from any other actors and stop it from being moved again
	LinkedObject->bCanMove = false;
	LinkedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	const FTransform MakeTransform(GetActorRotation(), GetActorLocation(), LinkedObject->GetActorScale3D());
	LinkedObject->SetActorTransform(MakeTransform);	
}

