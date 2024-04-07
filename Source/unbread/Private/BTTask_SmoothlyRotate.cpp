#include "BTTask_SmoothlyRotate.h"

#include "SRangedAICharacter.h"
#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

UBTTask_SmoothlyRotate::UBTTask_SmoothlyRotate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Smoothly Rotate";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_SmoothlyRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cntrl = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
	if (Cntrl)
	{
		MyActor = Cast<ASRangedAICharacter>(Cntrl->GetPawn());
		auto* TargetActor = Cast<AActor>(Cntrl->GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if (!MyActor)
		{
			return EBTNodeResult::Failed;
		}
		if (TargetActor)
		{
			TargetLoc = TargetActor->GetActorLocation();
		}
		else
		{
			TargetLoc = Cntrl->GetBlackboardComponent()->GetValueAsVector("MoveToLocation");
			if(TargetLoc == FVector())
			{
				return EBTNodeResult::Failed;
			}
		}
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}


void UBTTask_SmoothlyRotate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!MyActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	FVector Direction = (TargetLoc - MyActor->GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation();

	FRotator CurrentRotation = MyActor->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, InterpSpeed);
    
	MyActor->SetActorRotation({CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll});
    
	float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw));

	if (YawDifference < 5.0f) 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}