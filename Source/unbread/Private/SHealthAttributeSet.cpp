// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthAttributeSet.h"
#include "GameplayEffectExtension.h"

USHealthAttributeSet::USHealthAttributeSet()
	: Health(80.f), MaxHealth(100.f), Shield(0.0f), MaxShield(0.0f)
{
	
}

void USHealthAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// Super::ClampAttributeOnChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
}

void USHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetAppliedDamageAttribute())
	{
		float AppliedDamageDone = GetAppliedDamage();
		SetAppliedDamage(0.0f);
		if (AppliedDamageDone > 0.0f)
		{
			// Check if there's shield
			if (GetShield() > 0.0f)
			{
				float NewShield = GetShield();
				const float ShieldDifference = FMath::Min(NewShield, AppliedDamageDone);
				AppliedDamageDone -= ShieldDifference;
				NewShield -= ShieldDifference;
				SetShield(NewShield);
			}

			// Check if there's still damage to be applied after shield
			if (AppliedDamageDone > 0.0f)
			{
				float NewHealth = GetHealth();
				const float HealthDifference = FMath::Min(NewHealth, AppliedDamageDone);
				SetHealth(NewHealth - HealthDifference);
			}
		}
	}
}
