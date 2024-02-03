// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetRandomPoints.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API UBTTask_GetRandomPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_GetRandomPoints(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int32 Index = 0;
	
};
