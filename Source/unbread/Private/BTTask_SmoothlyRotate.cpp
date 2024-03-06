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
        MyActor = Cast<AActor>(Cntrl->GetPawn());
        TargetActor = Cast<AActor>(Cntrl->GetBlackboardComponent()->GetValueAsObject("TargetActor"));

        if (!MyActor || !TargetActor)
        {
            return EBTNodeResult::Failed;
        }
        
        return EBTNodeResult::InProgress;
    }
    return EBTNodeResult::Failed;
}

void UBTTask_SmoothlyRotate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!MyActor || !TargetActor)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector TargetLoc = TargetActor->GetActorLocation();
    FVector Direction = (TargetLoc - MyActor->GetActorLocation()).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();
    FRotator CurrentRotation = MyActor->GetActorRotation();    
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 5.0f);
    
    // Check if there's significant misalignment before rotating
    if (FMath::Abs(FVector::DotProduct(Direction, MyActor->GetActorForwardVector())) < 0.99f)
    {
        MyActor->SetActorRotation(NewRotation);
        // Task is still in progress because we haven't aligned yet
        // Do not call FinishLatentTask here to allow further ticks
    }
    else
    {
        // Alignment is good enough, finish task with success
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return; // Ensure no further code is executed after completing the task
    }
}

