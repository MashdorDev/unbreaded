// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAttack, // short range
	SecondaryAttack, // long range
	MovementAbility, // roll
	InteractionAbility, // push-pull-open-close etc
	UtilityAbility // swap ammo
	
	/*AllyAttack,
	AllyMovementAbility,
	AllyInteractionAbility,
	AllyUtilityAbility*/
};

