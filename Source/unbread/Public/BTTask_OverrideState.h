// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "unbread/unbread.h"
#include "BTTask_OverrideState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ERunMode : uint8
{
	ThisAgent		UMETA(DisplayName = "This Agent"),
	AllAgents		UMETA(DisplayName = "All Agents"),
};
UCLASS()
class UNBREAD_API UBTTask_OverrideState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_OverrideState(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	EAIState DesiredState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	ERunMode RunMode;
};
