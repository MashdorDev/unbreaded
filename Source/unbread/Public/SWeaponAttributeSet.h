// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseAttributeSet.h"
#include "AbilitySystemComponent.h"

#include "SWeaponAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API USWeaponAttributeSet : public USBaseAttributeSet
{
	GENERATED_BODY()

public:

	USWeaponAttributeSet();
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USWeaponAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FireRate;
	ATTRIBUTE_ACCESSORS(USWeaponAttributeSet, FireRate);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData LoadedAmmo;
	ATTRIBUTE_ACCESSORS(USWeaponAttributeSet, LoadedAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxLoadedAmmo;
	ATTRIBUTE_ACCESSORS(USWeaponAttributeSet, MaxLoadedAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CarriedAmmo;
	ATTRIBUTE_ACCESSORS(USWeaponAttributeSet, CarriedAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxCarriedAmmo;
	ATTRIBUTE_ACCESSORS(USWeaponAttributeSet, MaxCarriedAmmo);
	
};
