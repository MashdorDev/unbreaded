// Fill out your copyright notice in the Description page of Project Settings.


#include "SameFactionTest.h"

#include "AIController.h"
#include "SRangedAICharacter.h"
#include "unbread/unbread.h" // This should be the interface or class that provides the faction for an actor.

UEnvQueryTest_FactionComparison::UEnvQueryTest_FactionComparison(const FObjectInitializer& ObjectInitializer)
{
	
}

void UEnvQueryTest_FactionComparison::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	AAIController* AIController = Cast<AAIController>(QueryOwner);
	if (!AIController || !AIController->GetPawn())
	{
		return;
	}

	ASRangedAICharacter* OwnerFactionInterface = Cast<ASRangedAICharacter>(AIController->GetPawn());
	if (!OwnerFactionInterface)
	{
		return;
	}

	EFaction OwnerFaction = OwnerFactionInterface->faction;

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		AActor* TestActor = GetItemActor(QueryInstance, It.GetIndex());
		ASRangedAICharacter* TestActorFactionInterface = Cast<ASRangedAICharacter>(TestActor);

		bool test = (TestActorFactionInterface && TestActorFactionInterface->faction != OwnerFaction);
		
		It.SetScore(TestPurpose, FilterType, true, true);
	}
}
