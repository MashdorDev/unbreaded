// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindStrafeLocation.generated.h"

/**
 * 
 */
UCLASS()
class UNBREAD_API UBTTask_FindStrafeLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindStrafeLocation(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Blackboard")
	UEnvQuery* LocationSeekerQuery; // set the query in editor

	FEnvQueryRequest LocationSeekerQueryRequest;

	void LocaitonSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	class ASRanged_AIController* Cntrl;

	FVector StrafeLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Blackboard")
	float Distance = 500.0f;

	bool isDistanceGreaterThan(FVector Location);
	
};
