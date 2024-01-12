// Fill out your copyright notice in the Description page of Project Settings.


#include "SUnbreadPlayerHud.h"

#include "FrameTypes.h"
#include "Animation/AnimPhysicsSolver.h"
#include "Components/ProgressBar.h"

void USUnbreadPlayerHud::SetHealth(float CurrentHealth, float MaxHealth)
{
	if(HealthBar)
	{
		BarPercentage = CurrentHealth / MaxHealth;
		HealthBar->SetPercent(BarPercentage);
		TimeSinceHit = 0.0f;

	}
}

void USUnbreadPlayerHud::SetAmmunition(int CurrentAmmo, int MaxAmmo)
{
	Ammo = CurrentAmmo;	
	
	
}

void USUnbreadPlayerHud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	

	LerpDelayedHPBar(InDeltaTime);
	
	
}

void USUnbreadPlayerHud::NativeConstruct()
{
	Super::NativeConstruct();
	DelayBarPercentage = HealthBar->GetPercent();
	DelayedHealthBar->SetPercent(DelayBarPercentage);
	DelayBarLerpTarget = DelayBarPercentage;
	DelayBarLerpStart = DelayBarPercentage;
	DelayBarLerpT = 1.0f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Construct PlayerHud"));

}

void USUnbreadPlayerHud::LerpDelayedHPBar(const float& InDeltaTime)
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
		DelayBarLerpT -= 0.1f;
		// continue lerping
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
