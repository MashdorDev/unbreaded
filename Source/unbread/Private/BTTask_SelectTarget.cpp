// Fill out your copyright notice in the Description page of Project Settings.
 
#include "BTTask_SelectTarget.h"
#include "SRanged_AIController.h"
#include "SRangedAICharacter.h"
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
		EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_SelectTarget::EnemySeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_SelectTarget::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid() || !Cntrl || !Cntrl->BBC) return; // Safety check for null pointers

	BestTarget = nullptr;
	float CurrentBestScore = 0.0f;
	TArray<AActor*> AllDetectedActors;
	Result->GetAllAsActors(AllDetectedActors);

	for (int32 Index = 0; Index < AllDetectedActors.Num(); ++Index)
	{
		AActor* DetectedActor = AllDetectedActors[Index];
		ASCharacter* Character = Cast<ASCharacter>(DetectedActor);
		ASRangedAICharacter* RangedAICharacter = Cast<ASRangedAICharacter>(DetectedActor);

		if (Character)
		{
			// If it's an ASCharacter, prioritize it immediately without checking the score
			BestTarget = Character;
			break; // Breaks after finding the first ASCharacter
		}
		else if (RangedAICharacter && RangedAICharacter->faction != Cntrl->Agent->faction && !RangedAICharacter->Dead)
		{
			// For ASRangedAICharacter, check if it's the best target based on score
			float Score = Result->GetItemScore(Index);
			if (Score > CurrentBestScore && Score > 0.0f)
			{
				BestTarget = RangedAICharacter;
				CurrentBestScore = Score;
			}
		}
	}

	Cntrl->BBC->SetValueAsObject("TargetActor", BestTarget); // Set the target once after the loop
}
