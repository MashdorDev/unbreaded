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

UENUM(BlueprintType)
enum class EAIState : uint8 
{
	Idle		UMETA(DisplayName = "Idle"),
	ManDown		UMETA(DisplayName = "Man Down"),
	Investigate UMETA(DisplayName = "Investigate"),
	LostEnemy	UMETA(DisplayName = "LostEnemy"),
	Search		UMETA(DisplayName = "Search"),
	Attack		UMETA(DisplayName = "Attack"),
};

UENUM(BlueprintType)
enum class EFaction : uint8 
{
	Enemy		UMETA(DisplayName = "Enemy"),
	Friendly	UMETA(DisplayName = "Friendly"),
	Neutral		UMETA(DisplayName = "Neutral"),
};