// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SUnbreadPlayerHud.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNBREAD_API USUnbreadPlayerHud : public UUserWidget
{
	GENERATED_BODY()
public:
	// Update HUD with new HP
	UFUNCTION(BlueprintCallable)
	void SetHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	void SetAmmunition(int CurrentAmmo, int MaxAmmo);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, meta= (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta= (BindWidget))
	class UProgressBar* DelayedHealthBar;

	UPROPERTY(EditAnywhere)
	int Ammo;

private:
	float BarPercentage;
	float TimeSinceHit;
	float HPDelayThreshold;
	float DelayBarPercentage;

	
	
};
