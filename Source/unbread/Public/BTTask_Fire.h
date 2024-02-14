// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Fire.generated.h"

/**
 *
 */
UCLASS()
class UNBREAD_API UBTTask_Fire : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Fire(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void Shoot(bool NewBool);

	class ASRangedAICharacter* Chr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	bool ShouldFire = true;
};
