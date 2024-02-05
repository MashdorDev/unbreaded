// Fill out your copyright notice in the Description page of Project Settings.
 
#include "SRanged_AIController.h"
#include "SRangedAICharacter.h"
#include "BTTask_SelectTarget.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectTarget::UBTTask_SelectTarget(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Select Target";
}

EBTNodeResult::Type UBTTask_SelectTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cntrl = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
	if(Cntrl && EnemySeekerQuery)
	{
		EnemySeekerQueryRequest = FEnvQueryRequest(EnemySeekerQuery, Cntrl->Agent);
		EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_SelectTarget::EnemySeekerQueeyFinished);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_SelectTarget::EnemySeekerQueeyFinished(TSharedPtr<FEnvQueryResult> res)
{
	BestTarget = nullptr;
	float CurrentBEstScore = 0.0f;
	int32 Index = 0;
	TArray<AActor*> AllDetectedActors;
	res->GetAllAsActors(AllDetectedActors);
	Cntrl->BBC->SetValueAsObject("TargetActor", NULL);

	for(auto& DetectedActor: AllDetectedActors)
	{
		ASCharacter* chr = Cast<ASCharacter>(DetectedActor);
		if(chr)
		{
			if(res->GetItemScore(Index) > CurrentBEstScore && res->GetItemScore(Index) > 0.0f)
			{
				BestTarget = chr;
				CurrentBEstScore = res->GetItemScore(Index);
			}

			Index++;
		}
	}
	if(BestTarget)
	{
		Cntrl->BBC->SetValueAsObject("TargetActor", BestTarget);
	}
}
