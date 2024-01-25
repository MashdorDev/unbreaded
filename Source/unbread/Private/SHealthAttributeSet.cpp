// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthAttributeSet.h"

USHealthAttributeSet::USHealthAttributeSet()
	: Health(30.f), MaxHealth(70.f)
{
	
}

void USHealthAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// Super::ClampAttributeOnChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}
