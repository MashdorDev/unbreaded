// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetBlackboardKey.h"
#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBlackboardKey::UBTTask_SetBlackboardKey(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set BlackBoard Key";
}

EBTNodeResult::Type UBTTask_SetBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASRanged_AIController* MyController = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
	if(MyController)
	{
		MyController->BBC->SetValueAsBool("Damaged", Damage);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
