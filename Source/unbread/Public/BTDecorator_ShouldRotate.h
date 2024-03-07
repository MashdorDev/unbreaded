// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_ShouldRotate.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API UBTDecorator_ShouldRotate : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UBTDecorator_ShouldRotate(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Condition")
	float DeltaDegrees = 10.0f;
};
