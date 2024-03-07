// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_ShouldRotate.h"

#include "SRangedAICharacter.h"
#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_ShouldRotate::UBTDecorator_ShouldRotate(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Check Rotate";
	bCreateNodeInstance = true;
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}
bool UBTDecorator_ShouldRotate::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
	
	FVector MyLoc = MyController->Agent->GetActorLocation();
	FVector TargetLoc = EnemyActor->GetActorLocation();
	FVector Dir = (TargetLoc - MyLoc);
	
	if (FMath::Abs(FMath::FindDeltaAngleDegrees(MyController->Agent->GetActorRotation().Yaw, Dir.Rotation().Yaw)) >= DeltaDegrees)
	{
		OutsideRange = true;
	}

	return OutsideRange;
}

