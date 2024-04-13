// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetAnimationState.h"

#include "AIController.h"
#include "SRangedAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_SetAnimationState::UBTTask_SetAnimationState(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set Animation State";
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_SetAnimationState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	ASRangedAICharacter* chr = Cast<ASRangedAICharacter>(MyController->GetPawn());
	if(MyController && chr)
	{
		chr->ToggleCombat(Combat);

		const UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();
		auto myID = myBlackboard->GetKeyID(BlackboardKey.SelectedKeyName);

		AActor* EnemyActor = Cast<AActor>(myBlackboard->GetValue<UBlackboardKeyType_Object>(myID));
		if(EnemyActor)
		{
			(Focus) ? MyController->SetFocus(EnemyActor) : MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
			return EBTNodeResult::Succeeded;
		}
		MyController->ClearFocus((EAIFocusPriority::LastFocusPriority));
	}
	
	return EBTNodeResult::Succeeded;
}


