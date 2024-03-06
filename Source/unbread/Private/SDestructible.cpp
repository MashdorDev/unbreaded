// Fill out your copyright notice in the Description page of Project Settings.


#include "SDestructible.h"

#include "AbilitySystemComponent.h"
#include "SHealthAttributeSet.h"

// Sets default values
ASDestructible::ASDestructible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);

	HealthAttributeSet = CreateDefaultSubobject<USHealthAttributeSet>("HealthAttributeSet");

}

// Called when the game starts or when spawned
void ASDestructible::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComp)
	{
		return;
	}

	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ASDestructible::OnHealthAttributeChanged);
}

// Called every frame
void ASDestructible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ASDestructible::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ASDestructible::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

