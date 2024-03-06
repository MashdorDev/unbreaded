#include "BTTask_SmoothlyRotate.h"

#include "SRangedAICharacter.h"
#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTTask_SmoothlyRotate::UBTTask_SmoothlyRotate(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCreateNodeInstance = true;
    NodeName = "Smoothly Rotate";
}

EBTNodeResult::Type UBTTask_SmoothlyRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Cntrl = Cast<ASRanged_AIController>(OwnerComp.GetAIOwner());
    if (Cntrl)
    {
        TArray<AActor*> a;
        Cntrl->Agent->GetAttachedActors(a);
        if(a.Num() > 0)
        {
           int d = 0;
        }
        AActor* MyActor = Cast<AActor>(Cntrl->GetPawn()); // Ensure your AI controller's pawn is correctly set up to be your AI character.
        AActor* TargetActor = Cast<AActor>(Cntrl->GetBlackboardComponent()->GetValueAsObject("TargetActor"));

        if (!MyActor || !TargetActor)
        {
            return EBTNodeResult::Failed;
        }

        FVector MyLoc = MyActor->GetActorLocation();
        FVector TargetLoc = TargetActor->GetActorLocation();
        FRotator CurrentRotation = MyActor->GetActorRotation();

        FVector Direction = (TargetLoc - MyLoc).GetSafeNormal();
        FRotator TargetRotation = Direction.Rotation();

        // Determine the delta time since the last frame or task execution
        float DeltaTime = GetWorld()->DeltaTimeSeconds;
        
        // Smoothly interpolate to the new rotation
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f); // RotationSpeed is a factor controlling the rotation speed.

        MyActor->SetActorRotation({CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll});

        // Optionally, check if the rotation is close enough to the target rotation to consider the task done.
        if (FMath::Abs(FVector::DotProduct(Direction, MyActor->GetActorForwardVector())) > 0.8f) // Close to facing the target
        {
             return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed; // or EBTNodeResult::Failed if you want to fail the task when the rotation is not completed yet.
}
