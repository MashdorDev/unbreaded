// Fill out your copyright notice in the Description page of Project Settings.
 
#include "BTTask_SelectTarget.h"
#include "SRanged_AIController.h"
#include "SRangedAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectTarget::UBTTask_SelectTarget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
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
		EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_SelectTarget::EnemySeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_SelectTarget::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	// Early exit if pointers are invalid, streamlining the initial checks.
	if (!Result.IsValid() || !Cntrl || !Cntrl->BBC)
	{
		return;
	}

	BestTarget = nullptr;
	float HighestScore = FLT_MIN; // Use FLT_MIN to ensure any positive score is higher.

	// Directly iterating over the results, avoiding unnecessary array allocation.
	for (int32 Index = 0; Index < Result->Items.Num(); ++Index)
	{
		// Get the actor and score in a single loop to minimize calls and checks.
		AActor* Actor = Result->GetItemAsActor(Index);
		float Score = Result->GetItemScore(Index);

		ASRangedAICharacter* Character = Cast<ASRangedAICharacter>(Actor);
		if (Character && Character != Cntrl->Agent && Score > HighestScore)
		{
			// Update best target and highest score within the same condition.
			BestTarget = Character;
			HighestScore = Score;
		}
	}

	// Update the blackboard only if a best target was found.
	Cntrl->BBC->SetValueAsObject("TargetActor", BestTarget); // It's safe to set nullptr if no target is found.
}
