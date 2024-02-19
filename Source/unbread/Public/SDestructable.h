// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "SDestructable.generated.h"

class UAbilitySystemComponent;
class USHealthAttributeSet;

UCLASS()
class UNBREAD_API ASDestructable : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	USHealthAttributeSet* HealthAttributeSet;
	
public:	
	// Sets default values for this actor's properties
	ASDestructable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Inherited from IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnHealthChanged(float OldValue, float NewValue);

};
