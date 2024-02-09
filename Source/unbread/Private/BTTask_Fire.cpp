// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"

#include "SRanged_AIController.h"
#include "SRangedAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_Fire::UBTTask_Fire(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Fire Tim Bits";
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASRanged_AIController* MyController = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
	if(!MyController)
		return EBTNodeResult::Failed;

	Chr = Cast<ASRangedAICharacter>(MyController->GetPawn());
	if(!Chr)
		return EBTNodeResult::Failed;

	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	auto myID = MyBlackboard->GetKeyID(BlackboardKey.SelectedKeyName);

	ASRangedAICharacter* EnemyActor = Cast<ASRangedAICharacter>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(myID));
	if(!EnemyActor || EnemyActor->Dead || !ShouldFire)
	{
		Shoot(false);
		return EBTNodeResult::Succeeded;
	}

	FHitResult OutHit = MyController->Agent->CapsuleTrace();
	ASRangedAICharacter* HitActor = Cast<ASRangedAICharacter>(OutHit.GetActor());

	if(!HitActor || HitActor->faction == MyController->Agent->faction)
	{
		Shoot(false);
		MyController->BBC->SetValueAsBool("Move", true);
		return EBTNodeResult::Succeeded;
	}

	Shoot(true);
	return EBTNodeResult::Succeeded;
}

void UBTTask_Fire::Shoot(bool NewBool)
{
	Chr->ToggleADS(NewBool);
	if(NewBool)
	{
		if(!Chr->FireHandle.IsValid())
		{
			Chr->StartWaponFire();
		}
		return;
	}
	Chr->StopWeaponFire();
}
