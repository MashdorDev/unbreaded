// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SmoothlyRotate.generated.h"

class ASRangedAICharacter;
/**
 * 
 */
UCLASS()
class UNBREAD_API UBTTask_SmoothlyRotate : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SmoothlyRotate(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	class ASRanged_AIController* Cntrl;
	ASRangedAICharacter* MyActor = nullptr;
	FVector TargetLoc = FVector();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float InterpSpeed = 5.0f;
};