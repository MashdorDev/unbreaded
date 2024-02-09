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
	float HighestScore = -1.0f;
	
	for (int32 Index = 0; Index < Result->Items.Num(); ++Index)
	{
		
		AActor* Actor = Result->GetItemAsActor(Index);
		float Score = Result->GetItemScore(Index);

		ASCharacter* player = Cast<ASCharacter>(Actor);
		ASRangedAICharacter* Character = Cast<ASRangedAICharacter>(Actor);
		if(player)
		{
			BestTarget = player;
			HighestScore = Score;
			break;
		}
		if (Character && Character->faction != Cntrl->Agent->faction && Score > HighestScore)
		{
			BestTarget = Character;
			HighestScore = Score;
		}
	}
	
	Cntrl->BBC->SetValueAsObject("TargetActor", BestTarget); 
}
