// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_OverrideState.h"

#include "AIManager.h"
#include "SRanged_AIController.h"
#include "SRangedAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

UBTTask_OverrideState::UBTTask_OverrideState(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "AIState Override";
}

EBTNodeResult::Type UBTTask_OverrideState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASRanged_AIController* MyController = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
	if(!MyController)
	{
		return EBTNodeResult::Failed;
	}

	if (RunMode == ERunMode::ThisAgent)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>("AIState", (uint8_t)DesiredState);
	}
	else
	{
		MyController->AIManager->NotifyAIState(DesiredState);
	}

	return EBTNodeResult::Succeeded;
}
