// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "SPlayerHealthBar.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS(Blueprintable)
class UNBREAD_API USPlayerHealthBar : public UCanvasPanel
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable)
	void SetHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	void SetAmmunition(int CurrentAmmo, int MaxAmmo = 3);

	UFUNCTION(BlueprintCallable)
	void OnTick(const float InDeltaTime);

	UFUNCTION(BlueprintCallable)
	void OnConstruct();

	void LerpDelayedHPBar(const float& InDeltaTime);

	void LerpHealthBar(const float InDeltaTime);


	UPROPERTY(EditAnywhere)
	int Ammo;
	
	UPROPERTY(EditAnywhere, meta= (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta= (BindWidget))
	class UProgressBar* DelayedHealthBar;

	UPROPERTY(EditAnywhere)
	float HPDelayThreshold;

private:
	float BarPercentage;
	float TimeSinceHit;
	
	float DelayBarPercentage;
	float DelayBarLerpT;
	float DelayBarLerpStart;
	float DelayBarLerpTarget;
	bool IsLerping;
};