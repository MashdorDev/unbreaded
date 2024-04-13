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

	
	if(pathRef->Locations.Num() - 1 > 0)
	{
		int32 nextPoint = 0;
		Index = GetNextPoint(pathRef, nextPoint);
		
		pathRef->Locations[Index].isOccupied = true;
		pathRef->Locations[PreviousIndex].isOccupied = false;
		
		PreviousIndex = Index;
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", pathRef->Locations[Index].Location);
		return EBTNodeResult::Succeeded;
	}
	
	Index = 0;
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", pathRef->Locations[Index].Location);
	return EBTNodeResult::Succeeded;
}

int32_t UBTTask_GetRandomPoints::GetNextPoint(ASPathActor* pathRef, int32& nextPoint)
{
	nextPoint = FMath::RandRange(0, pathRef->Locations.Num() - 1);
	if(pathRef->Locations[nextPoint].isOccupied)
	{
		nextPoint = GetNextPoint(pathRef, nextPoint);
	}
	return nextPoint;
}
