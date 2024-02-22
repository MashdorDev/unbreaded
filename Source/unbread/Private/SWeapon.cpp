// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "Components/SphereComponent.h"

// Sets default values
ASWeapon::ASWeapon()
	: Damage(10.0f), FireRate(1.0f), bIsAutoFireOn(false)
{
 	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
	RootComponent = SkeletalMeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Equip()
{
	OnEquip();
}

void ASWeapon::UnEquip()
{
	OnUnequip();
}

