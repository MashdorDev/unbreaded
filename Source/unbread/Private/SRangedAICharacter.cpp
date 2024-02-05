// Fill out your copyright notice in the Description page of Project Settings.


#include "SRangedAICharacter.h"

#include "AIManager.h"
#include "SRanged_AIController.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASRangedAICharacter::ASRangedAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// might change this
bool ASRangedAICharacter::IsHostile(ASCharacter* character)
{
	if(character)
	{
		return true;
	}
	return false;
}


void ASRangedAICharacter::MakeSound(FVector Locaiton)
{
	MakeNoise(1.0f, nullptr, Locaiton, 0.0f, "");
}

// Called when the game starts or when spawned
void ASRangedAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float ASRangedAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	Health -= DamageApplied;

	if (DamageApplied > 0)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, BloodFX, GetActorLocation());
	}

	if (Health <= 0)
	{
		GetMesh()->bIgnoreRadialForce = true;
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Dead= true;
		if(ControllerRef)
		{
			ControllerRef->GetBrainComponent()->StopLogic("Agent is Dead");
			ControllerRef->ClearFocus(EAIFocusPriority::LastFocusPriority);
			ControllerRef->GetAIPerceptionComponent()->DestroyComponent(true);
			ControllerRef->AIManager->RemoveAgent(ControllerRef);
		}
		return 0.0f;
	}

	// Return the amount of damage applied
	return DamageApplied;
}


bool ASRangedAICharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation,
                                        int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible,
                                        int32* UserData) const
{
	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));
	
	FHitResult HitResult;
	FVector SocketLocaiton = GetMesh()->GetSocketLocation(PerceptionTarget);

	const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SocketLocaiton,
		FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)),
		FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if(bHitSocket == false || (HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = SocketLocaiton;
		OutSightStrength = 1;

		return true;
	}

	const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation(),
		FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)),
		FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	NumberOfLoSChecksPerformed++;
	
	if(bHitSocket == false || (HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;

		return true;
	}
	OutSightStrength = 0;
	
	return false;
}

// Called every frame
void ASRangedAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASRangedAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ASRangedAICharacter::GetBehaviourTree() const
{
	return Tree;
}

void ASRangedAICharacter::ToggleCombat(const bool Newbool)
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	AnimValues.bIsInCombat = Newbool;
	bUseControllerRotationYaw = Newbool;
	GetCharacterMovement()->bOrientRotationToMovement = !Newbool;
	GetCharacterMovement()->MaxWalkSpeed = (Newbool) ? 187.f : 94.f;
}

void ASRangedAICharacter::ToggleCrouch(const bool Newbool)
{
	AnimValues.bIsCrouching = Newbool;
	const float Speed = AnimValues.bIsInCombat ? 187.f : WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = (Newbool) ? CrouchedWalkSpeed : Speed;
}

void ASRangedAICharacter::ToggleADS(const bool Newbool)
{
	AnimValues.bADS = Newbool;
}


