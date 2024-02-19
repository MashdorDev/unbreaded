// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomPoints.h"

#include "AIController.h"
#include "SRangedAICharacter.h"
#include "SPathActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetRandomPoints::UBTTask_GetRandomPoints(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Path Points";
}

EBTNodeResult::Type UBTTask_GetRandomPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* myBlackBoard = OwnerComp.GetBlackboardComponent();
	const AAIController* myController = OwnerComp.GetAIOwner();
	if(!myController || !myBlackBoard)
	{
		return EBTNodeResult::Failed;
	}

	const ASRangedAICharacter* character = Cast<ASRangedAICharacter>(myController->GetPawn());
	if(!character)
	{
		return EBTNodeResult::Failed;
	}

	ASPathActor* pathRef = Cast<ASPathActor>(character->SmartObject);
	if(!pathRef || pathRef->Locations.Num() < 1)
	{
		return EBTNodeResult::Succeeded;
	}

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", pathRef->Locations[Index]);
	if(Index < pathRef->Locations.Num() - 1)
	{
		Index = GetNextPoint(pathRef->Locations.Num() - 1);
		return EBTNodeResult::Succeeded;
	}
	Index = 0;
	
	return EBTNodeResult::Succeeded;
}

int32_t UBTTask_GetRandomPoints::GetNextPoint(int32 MaxNum)
{
	int32 nextPoint = FMath::RandRange(0, MaxNum);
	if(nextPoint == Index)
	{
		nextPoint = GetNextPoint(MaxNum);
	}

	return nextPoint;
}
