// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_DistanceCheck.h"

#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_DistanceCheck::UBTDecorator_DistanceCheck(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Distance Check";
	bCreateNodeInstance = true;
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}
bool UBTDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	const ASRanged_AIController* MyController = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
	bool OutsideRange = false;

	if(!MyController || !MyBlackboard)
		return OutsideRange;

	const auto MyID = MyBlackboard->GetKeyID(BlackboardKey.SelectedKeyName);
	UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(MyID);
	const AActor* EnemyActor = Cast<AActor>(KeyValue);
	if(!EnemyActor)
	{
		return OutsideRange;
	}
	
	if(MyController->GetPawn()->GetDistanceTo(EnemyActor) >= TestDistance)
	{
		OutsideRange = true;
	}
	return OutsideRange;
}

