// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "Perception/AISightTargetInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "SRanged_AIController.generated.h"


/**
 * 
 */
UCLASS()
class UNBREAD_API ASRanged_AIController : public AAIController
{
	GENERATED_BODY()
public:
	ASRanged_AIController(FObjectInitializer const& ObjectInitializer);

	void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	class AAIManager* AIManager = nullptr;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(transient)
	class UBehaviorTreeComponent* BTC;

	UFUNCTION(BlueprintCallable, Category="AI")
	float GetDetectionLevel() const;
	
	UPROPERTY(transient)
	class UBlackboardComponent*  BBC;

	class ASRangedAICharacter* Agent;

	UPROPERTY(BlueprintReadWrite)
	float DetectionLevel = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float TimeStamp = 0.0f;
	
	// Use FBlackboard::FKey for the key type
	FBlackboard::FKey EnemyKeyID;
	FBlackboard::FKey LocationKeyID;
	FBlackboard::FKey ContactKeyID;
	FBlackboard::FKey DamagedKeyID;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectionThreshold = 5.0f;
protected:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AiPerceptionComponent;

	UFUNCTION()
	void OnPerception(AActor* actor, FAIStimulus stimulus);
	
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

	FTimerHandle DetectionTimer;
	void SetDetectionLevel();
	float Rate = 1.0f;
	
	AActor* Target = nullptr;
	FVector LastStimulusLocation = FVector::ZeroVector;
};