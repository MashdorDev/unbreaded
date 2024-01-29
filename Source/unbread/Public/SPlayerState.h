// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "SPlayerState.generated.h"

class UAbilitySystemComponent;
class USHealthAttributeSet;

/**
 * 
 */
UCLASS()
class UNBREAD_API ASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASPlayerState();

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	USHealthAttributeSet* HealthAttributeSet;

protected:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	
};