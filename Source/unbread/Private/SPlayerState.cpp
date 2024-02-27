// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "AbilitySystemComponent.h"
#include "SHealthAttributeSet.h"
#include "SWeaponAttributeSet.h"

ASPlayerState::ASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	//AbilitySystemComponent->SetIsReplicated(true);

	HealthAttributeSet = CreateDefaultSubobject<USHealthAttributeSet>("HealthAttributeSet");
	WeaponAttributeSet = CreateDefaultSubobject<USWeaponAttributeSet>("WeaponAttributeSet");
}

UAbilitySystemComponent* ASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
