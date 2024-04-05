// Fill out your copyright notice in the Description page of Project Settings.


#include "SRanged_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SRangedAICharacter.h"
#include "S_SmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SCharacter.h"
#include "AIManager.h"	
#include "unbread/unbread.h"

ASRanged_AIController::ASRanged_AIController(FObjectInitializer const& ObjectInitializer)
{
	BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree Component"));

	AiPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sigth Config"));
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));

	Sight->SightRadius = 2000.0f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.0f;
	Hearing->HearingRange = 2000.0f;

	//Tell the senses to detect everything
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	// Register the sight sense to our perception Component
	AiPerceptionComponent->ConfigureSense(*Sight);
	AiPerceptionComponent->ConfigureSense(*Hearing);
	AiPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
}

void ASRanged_AIController::BeginPlay()
{
	Super::BeginPlay();
	if(!Agent)
	{
		ASRangedAICharacter* character = Cast<ASRangedAICharacter>(GetPawn());
		if(!character)
		{
			return;
		}
		Agent = character;
		Agent->ControllerRef = this;
	}

	if(Agent->SmartObject)
	{
		FGameplayTag SubTag;
		BTC->SetDynamicSubtree(SubTag, Agent->SmartObject->SubTree);
	}
	BBC->SetValueAsEnum("CombatRole", static_cast<uint8>(Agent->CombatRole));
}

// Implement these in your .cpp file
float ASRanged_AIController::GetDetectionLevel() const
{
	return DetectionLevel;
}

void ASRanged_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ASRangedAICharacter*  aiCharacter = Cast<ASRangedAICharacter>(InPawn);
	
	if(aiCharacter != nullptr && aiCharacter->GetBehaviourTree() != nullptr)
	{
		Agent = aiCharacter;
		Agent->ControllerRef = this;
		
		BBC->InitializeBlackboard(*aiCharacter->GetBehaviourTree()->BlackboardAsset);

		EnemyKeyID = BBC->GetKeyID("TargetActor");
		LocationKeyID = BBC->GetKeyID("MoveToLocation");
		ContactKeyID = BBC->GetKeyID("Contact");
		DamagedKeyID = BBC->GetKeyID("Damaged");
		StateID = BBC->GetKeyID("EAIState");

		BTC->StartTree(*aiCharacter->GetBehaviourTree());
		
	}
	AiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASRanged_AIController::OnPerception);
}

void ASRanged_AIController::OnPerception(AActor* actor, FAIStimulus stimulus)
{
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), stimulus) == UAISense_Sight::StaticClass())
	{
		ASCharacter* chr = Cast<ASCharacter>(actor);
		ASRangedAICharacter* chr2 = Cast<ASRangedAICharacter>(actor);
        
		// Improve logical condition clarity and correctness
		bool isEnemy = chr || (chr2 && chr2->faction != Agent->faction && chr2->faction != EFaction::Neutral);
        
		if (isEnemy)
		{
			BBC->SetValueAsBool("Contact", stimulus.WasSuccessfullySensed());
            
			if (BBC->GetValueAsEnum("AIState") != static_cast<uint8>(EAIState::Attack))
			{
				BBC->SetValueAsObject("TargetActor", actor);
			}	

			Target = actor;
			LastStimulusLocation = stimulus.StimulusLocation;
			TimeStamp = UKismetSystemLibrary::GetGameTimeInSeconds(Agent);

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Time is: %f"), TimeStamp));

			// Correct the logical AND operator
			if (!GetWorldTimerManager().IsTimerActive(DetectionTimer) && BBC->GetValueAsBool("Contact") && BBC->GetValueAsEnum("AIState") == static_cast<uint8>(EAIState::Idle))
			{
				DetectionLevel = 0.0f;
				Agent->UpdateWidgetVis(true);
				GetWorldTimerManager().SetTimer(DetectionTimer, this, &ASRanged_AIController::SetDetectionLevel, 0.1f, true, 0.f);
			}
			return;
		}
	}
	
	if(BBC->GetValueAsEnum("AIState") == (uint8_t)EAIState::Attack)
	{
		return;
	}

	// might change here
	ASCharacter* chr = Cast<ASCharacter>(actor);
	ASRangedAICharacter* chr2 = Cast<ASRangedAICharacter>(actor);
	if(chr || (chr2 && chr2->faction != Agent->faction && chr2->faction != EFaction::Neutral))
	{
		BBC->SetValueAsEnum("AIState", (uint8_t)EAIState::Investigate);
		BBC->SetValueAsVector("MoveToLocation", stimulus.StimulusLocation);
	}
}

void ASRanged_AIController::SetDetectionLevel() 
{
	auto State = BBC->GetValueAsEnum("AIState");

	if(!Target || !BBC->GetValueAsBool("Contact"))
	{
		if(State != (uint8_t)EAIState::Idle)
		{
			GetWorldTimerManager().ClearTimer(DetectionTimer);
			Agent->UpdateWidgetVis(false);
			return;
		}

		if(DetectionLevel > 0.0f)
		{
			DetectionLevel -= 1;
			return;
		}
		GetWorldTimerManager().ClearTimer(DetectionTimer);
		Agent->UpdateWidgetVis(false);

		return;
	}
	
	const float Distance = GetPawn()->GetDistanceTo(Target);

	if (Distance <= 2000.0f) {
		Rate =  Distance / 2000.0f;
	} else {
		Rate = 1.0f / (Distance / 2000.0f);
	}

	DetectionLevel += 0.2f / Rate;
	
	if(DetectionLevel >= DetectionThreshold)
	{
		AIManager->NotifyAIState(EAIState::Attack);
		GetWorldTimerManager().ClearTimer(DetectionTimer);
		Agent->UpdateWidgetVis(false);
		return;
	}
	if(DetectionLevel >= DetectionThreshold / 2)
	{
		BBC->SetValueAsEnum("AIState", (uint8_t)EAIState::Investigate);
		BBC->SetValueAsVector("MoveToLocation", LastStimulusLocation);
	}
}

