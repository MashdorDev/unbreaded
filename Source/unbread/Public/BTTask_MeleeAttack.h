
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MeleeAttack.generated.h"

UCLASS()
class UNBREAD_API UBTTask_MeleeAttack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_MeleeAttack(const FObjectInitializer& ObjectInitializer);

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // Add your properties like AttackRange here
    UPROPERTY(EditAnywhere, Category = "AI");
    float AttackRange = 100.0f; // Example range, adjust as necessary

    // Add other properties like attack damage, cooldown duration, etc.
};

/*
 The UBTTask_MeleeAttack class is a simple task that checks if the target is within a certain range and performs an attack if it is. The ExecuteTask method is where the actual logic is implemented. The UBTTask_MeleeAttack class is derived from UBTTaskNode, which is the base class for all behavior tree tasks.
 The UBTTask_MeleeAttack class has a single property, AttackRange, which is the range at which the AI character can perform a melee attack. This property is exposed to the editor and can be adjusted as necessary.
 The ExecuteTask method is where the actual logic for the melee attack is implemented. The method first checks if the AI controller and pawn are valid. It then checks if the target actor is within the specified range. If the target is within range, the method performs the attack and returns EBTNodeResult::Succeeded. If the target is not within range, the method returns EBTNodeResult::Failed.
 The UBTTask_MeleeAttack class can be used in a behavior tree to implement melee attacks for AI characters.

    */