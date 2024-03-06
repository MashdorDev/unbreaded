// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerHealthBar.h"

#include "Components/ProgressBar.h"

void USPlayerHealthBar::SetHealth(float CurrentHealth, float MaxHealth)
{
	if(!HealthBar) return;

	BarPercentage = CurrentHealth / MaxHealth;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HEALTH UI: %f"), BarPercentage));

	HealthBar->SetPercent(BarPercentage);
	TimeSinceHit = 0.0f;
}

void USPlayerHealthBar::SetShield(float CurrentShield, float MaxShield)
{
	ShieldPercentage = CurrentShield / MaxShield;
	ShieldBar->SetPercent(ShieldPercentage);
}

void USPlayerHealthBar::SetAmmunition(int CurrentAmmo, int MaxAmmo)
{
	Ammo = CurrentAmmo;
	
}

void USPlayerHealthBar::OnTick(const float InDeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Ticking hp"));

	LerpDelayedHPBar(InDeltaTime);
}

void USPlayerHealthBar::OnConstruct()
{
	DelayBarPercentage = HealthBar->GetPercent();
	DelayedHealthBar->SetPercent(DelayBarPercentage);
	DelayBarLerpTarget = DelayBarPercentage;
	DelayBarLerpStart = DelayBarPercentage;
	DelayBarLerpT = 1.0f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Construct health bar"));
}

void USPlayerHealthBar::LerpDelayedHPBar(const float& InDeltaTime)
{
	if(!IsLerping)
	{
		TimeSinceHit += InDeltaTime;
		
		if(TimeSinceHit >= HPDelayThreshold)
		{
			// start lerping hp bar, reset all values
			IsLerping = true;
			DelayBarLerpT = 1.0f;
			DelayBarLerpTarget = BarPercentage;
        		
		}
		return;
	}
	
	if(DelayBarLerpT >= 0.0f)
	{
		// continue lerping
		DelayBarLerpT -= 0.1f;
		DelayBarPercentage = FMath::Lerp(DelayBarLerpTarget, DelayBarLerpStart, DelayBarLerpT);
		DelayedHealthBar->SetPercent(DelayBarPercentage);
		
	}
	else
	{
		// done lerp
		IsLerping = false;
		DelayBarLerpStart = DelayBarPercentage;
	}
}

void USPlayerHealthBar::LerpHealthBar(const float InDeltaTime)
{
}
