// Fill out your copyright notice in the Description page of Project Settings.


#include "SRangedAICharacter.h"

#include "AIManager.h"
#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IO/IoDispatcher.h"

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


FHitResult ASRangedAICharacter::CapsuleTrace()
{
	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FVector EyesLoc;
	FRotator EyesRot;
	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	const FVector End = (EyesRot.Vector() * 2000.0f) + EyesLoc + FVector(0.0f, 0.0f,120.0f);
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), EyesLoc, End, 20.0f, ETraceTypeQuery::TraceTypeQuery_MAX, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Green);
	return OutHit;
}

FHitResult ASRangedAICharacter::TraceProvider(FVector Start, FVector End)
{
	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FVector EyesLoc;
	FRotator EyesRot;
	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	End = (EyesRot.Vector() * 2000.0f) + EyesLoc + FVector(0.0f, 0.0f,120.0f);
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), EyesLoc, End, 20.0f, ETraceTypeQuery::TraceTypeQuery_MAX, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FColor::Green);
	return OutHit;
}

void ASRangedAICharacter::StartWaponFire()
{
	if(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}
	if(!AnimValues.bIsInCombat)
	{
		return;
	}

	ToggleADS(true);
	AnimValues.bIsShooting = true;

	FVector EyesLoc;
	FRotator EyesRot;
	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	const FVector End = EyesRot.Vector() * 2000.0f + EyesLoc;

	FHitResult HitInfo = TraceProvider(EyesLoc, End);

	if (!HitInfo.bBlockingHit)
		return;

	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), BaseDamage, HitInfo.ImpactPoint, HitInfo, this->GetController(), this, nullptr);

	//DrawDebugLine(GetWorld(), EyesLoc, End, FColor::Green, false, 1.0f, 0, 1.0f);

	
	if(FireHandle.IsValid())
	{
		return;
	}
	GetWorldTimerManager().SetTimer(FireHandle, this, &ASRangedAICharacter::StartWaponFire, FireRate, true, 0.0f);
}

void ASRangedAICharacter::StopWeaponFire()
{
	GetWorldTimerManager().ClearTimer(FireHandle);
	AnimValues.bIsShooting = false;
}

void ASRangedAICharacter::MakeSound(FVector Locaiton)
{
	MakeNoise(1.0f, nullptr, Locaiton, 0.0f, "");
}

// Called when the game starts or when spawned
void ASRangedAICharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateWindgerRef();
}

float ASRangedAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	Health -= DamageApplied;

	if(ControllerRef)
	{
		ControllerRef->BBC->SetValueAsBool("Damaged", true);
	}

	if (DamageApplied > 0)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, BloodFX, GetActorLocation());
	}

	if (Health <= 0)
	{
		GetMesh()->bIgnoreRadialForce = true;
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Dead = true;
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

