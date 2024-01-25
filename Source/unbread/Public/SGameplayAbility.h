// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "unbread/unbread.h"
#include "SGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API USGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID { EAbilityInputID::None };	
	
};
