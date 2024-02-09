// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetBlackboardKey.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API UBTTask_SetBlackboardKey : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SetBlackboardKey(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	bool Damage = false;
};
