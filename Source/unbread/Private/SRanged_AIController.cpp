// Fill out your copyright notice in the Description page of Project Settings.


#include "SRanged_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SRangedAICharacter.h"
#include "S_SmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ASRanged_AIController::ASRanged_AIController(FObjectInitializer const& ObjectInitializer)
{
	BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree Component"));
	AiPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sigth Config"));
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));

	Sight->SightRadius = 2000.0f;
	Sight->LoseSightRadius = Sight->SightRadius +500.0f;
	Hearing->HearingRange = 2000.0f;

	//Tell the senses to detect everthing
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	// REgister the sight sense to our perception Component
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
		//Agent->ControllerRef = this;
	}

	if(Agent->SmartObject)
	{
		FGameplayTag SubTag;
		BTC->SetDynamicSubtree(SubTag, Agent->SmartObject->SubTree);
	}
}

void ASRanged_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ASRangedAICharacter*  aiCharacter = Cast<ASRangedAICharacter>(InPawn);
	
	if(aiCharacter != nullptr && aiCharacter->GetBehaviourTree() != nullptr)
	{
		Agent = aiCharacter;
		//Agent->ControllerRef = this;
		
		BBC->InitializeBlackboard(*aiCharacter->GetBehaviourTree()->BlackboardAsset);

		EnemyKeyID = BBC->GetKeyID("TargetActor");
		LocationKeyID = BBC->GetKeyID("MoveToLocation");

		BTC->StartTree(*aiCharacter->GetBehaviourTree());
		
	}
}

void ASRanged_AIController::OnPerception(AActor* actor, FAIStimulus stimulus)
{
	if(UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), stimulus) == UAISense_Sight::StaticClass())
	{
		
	}
	
}
