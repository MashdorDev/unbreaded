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

	int32_t Index = 0;
	int32_t PreviousIndex = 0;
private:
	
	int32_t GetNextPoint(class ASPathActor* pathRef, int32& nextPoint);
};
