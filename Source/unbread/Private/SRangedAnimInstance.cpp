// Fill out your copyright notice in the Description page of Project Settings.


#include "SRangedAnimInstance.h"

#include "SRangedAICharacter.h"

USRangedAnimInstance::USRangedAnimInstance()
{
}

void USRangedAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ASRangedAICharacter>(TryGetPawnOwner());
}

void USRangedAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character || DeltaSeconds == 0.0f)
	{
		return;
	}

	FVector NewVelocity = Character->GetVelocity();
	
	Move = FVector::DotProduct(Character->GetActorForwardVector(), Character->GetVelocity().GetSafeNormal()) > 0 ? 1.0f: -1.0f;
	Speed = NewVelocity.Length();
	SetCharacterProperties();
}

void USRangedAnimInstance::SetCharacterProperties()
{
	bHasPath = Character->SmartObject != nullptr;
	bHasDetected = Character->detectedPlayer;
	bIsShooting = Character->AnimValues.bIsShooting;
}
