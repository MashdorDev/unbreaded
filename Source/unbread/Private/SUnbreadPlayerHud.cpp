// Fill out your copyright notice in the Description page of Project Settings.


#include "SUnbreadPlayerHud.h"

#include "Components/ProgressBar.h"

void USUnbreadPlayerHud::SetHealth(float CurrentHealth, float MaxHealth)
{
	if(HealthBar)
	{
		BarPercentage = CurrentHealth / MaxHealth;
		HealthBar->SetPercent(BarPercentage);
	}
}

void USUnbreadPlayerHud::SetAmmunition(int CurrentAmmo, int MaxAmmo)
{
	Ammo = CurrentAmmo;	
	
	
}

void USUnbreadPlayerHud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	
	
}
