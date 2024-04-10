// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::MeleeInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	//ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	//ObjectQueryParams.AddObjectTypesToQuery(ECC_EngineTraceChannel3);
	
	AActor* MyOwner = GetOwner();
	
	ACharacter* MyCharacter = Cast<ACharacter>(MyOwner);
	if (MyCharacter)
	{
		USkeletalMeshComponent* CharacterMesh = MyCharacter->GetMesh();
		if (CharacterMesh)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			CharacterMesh->GetSocketWorldLocationAndRotation(TEXT("spine_004"), EyeLocation, EyeRotation);
			FRotator Correction (0.0f, 90.0f, 0.f);
			FRotator CorrecterRotation = EyeRotation + Correction;
			FVector End = EyeLocation + (CorrecterRotation.Vector() * 300);
			End.Z = EyeLocation.Z;
			// FHitResult Hit;
			// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

			TArray<FHitResult> Hits;

			float DebugRadius = 30.f;

			FCollisionShape CollisionShape;
			CollisionShape.SetSphere(DebugRadius);

			bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
			FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
			for (FHitResult Hit : Hits)
			{
				if (AActor* HitActor = Hit.GetActor())
				{
					if(HitActor->Implements<USGameplayInterface>())
					{
						APawn* MyPawn = Cast<APawn>(MyOwner);
						ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
						break;
					}
				}
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, DebugRadius, 16, LineColor, false, 2.0f);
			}
			DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	
		}
	}

}
