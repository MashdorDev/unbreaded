// Fill out your copyright notice in the Description page of Project Settings.


#include "SDestructable.h"

#include "AbilitySystemComponent.h"
#include "SHealthAttributeSet.h"

// Sets default values
ASDestructable::ASDestructable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	//AbilitySystemComp->SetIsReplicated(true);

	HealthAttributeSet = CreateDefaultSubobject<USHealthAttributeSet>("HealthAttributeSet");

}

// Called when the game starts or when spawned
void ASDestructable::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComp)
	{
		return;
	}

	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ASDestructable::OnHealthAttributeChanged);
	
}

// Called every frame
void ASDestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ASDestructable::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ASDestructable::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

