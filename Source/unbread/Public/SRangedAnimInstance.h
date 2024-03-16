// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SRangedAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API USRangedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	USRangedAnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetCharacterProperties();
	
	UPROPERTY(BlueprintReadOnly)
	class ASRangedAICharacter* Character = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Speed = 0.f;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Move = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bHasPath = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bIsShooting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bHasDetected = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bHasProjectile = false;
	
};
