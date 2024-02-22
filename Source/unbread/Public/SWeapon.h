// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USphereComponent;

UCLASS()
class UNBREAD_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true));
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = true))
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = true))
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = true))
	bool bIsAutoFireOn;
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// C++ functions
	void Equip();
	void UnEquip();

	// Blueprint functions
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipping")
	void OnEquip();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipping")
	void OnUnequip();

};
