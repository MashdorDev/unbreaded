// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetAnimationState.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API UBTTask_SetAnimationState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SetAnimationState(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	bool Combat = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	bool Focus = false;
	
};
