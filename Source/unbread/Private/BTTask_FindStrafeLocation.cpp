// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindStrafeLocation.h"
#include "SRanged_AIController.h"
#include "SRangedAICharacter.h"
#include "AIManager.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_FindStrafeLocation::UBTTask_FindStrafeLocation(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Find Strafe Location";
}

EBTNodeResult::Type UBTTask_FindStrafeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cntrl = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
	if(Cntrl && LocationSeekerQuery)
	{
		LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, Cntrl->Agent);
		LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_FindStrafeLocation::LocaitonSeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_FindStrafeLocation::LocaitonSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	int32 Index = 0;
	float CurrentBestScore = 0.0f;
	TArray<FVector> Locations;
	Result->GetAllAsLocations(Locations);
	for(auto& Loc: Locations)
	{
		if(isDistanceGreaterThan(Loc) && Result->GetItemScore(Index) > CurrentBestScore)
		{
			StrafeLocation = Loc;
			CurrentBestScore = Result->GetItemScore(Index);
		}
		Index++;
	}
	Cntrl->BBC->SetValueAsVector("MoveToLocation", StrafeLocation);
}

bool UBTTask_FindStrafeLocation::isDistanceGreaterThan(FVector Location)
{
	if(Cntrl->AIManager->Agents.Num() <= 1) return true;
	bool ConsiderLocation = true;
	for(auto& Agnt: Cntrl->AIManager->Agents)
	{
		const float CalculatedDistance = (Location - Agnt->Agent->GetActorLocation()).Size();
		if(CalculatedDistance <= Distance)
		{
			ConsiderLocation = false;
			break;
		}
	}
	return ConsiderLocation;
}
