// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SHealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API USHealthAttributeSet : public USBaseAttributeSet
{
	GENERATED_BODY()

public:
	USHealthAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USHealthAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USHealthAttributeSet, MaxHealth);

protected:

	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
};
