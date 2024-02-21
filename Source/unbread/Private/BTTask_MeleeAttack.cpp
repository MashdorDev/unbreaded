// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

#include "BTTask_MeleeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_MeleeAttack::UBTTask_MeleeAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    NodeName = "Melee Attack";
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (APawn* Pawn = AIController->GetPawn())
        {
            if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
            {
                if (AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor")))
                {
                    const float DistanceToTarget = FVector::Dist(Pawn->GetActorLocation(), TargetActor->GetActorLocation());

                    if (DistanceToTarget <= AttackRange)
                    {
                        // Perform attack here (e.g., apply damage to TargetActor, play animation)

                        // Set cooldown in Blackboard if needed
                        return EBTNodeResult::Succeeded;
                    }
                }
            }
        }
    }
    return EBTNodeResult::Failed;
}
