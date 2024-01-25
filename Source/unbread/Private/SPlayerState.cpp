// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "AbilitySystemComponent.h"

ASPlayerState::ASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	//AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* ASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
