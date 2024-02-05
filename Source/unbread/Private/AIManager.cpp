// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"

#include "SRangedAICharacter.h"
#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	CreateAgentsList();
}

bool AAIManager::Engaged()
{
	bool ED = false;

	for(auto& Cntrl : Agents)
	{
		if(Cntrl->BBC->GetValueAsBool("Contact"))
		{
			ED = true;
			break;
		}
		if(Cntrl->BBC->GetValueAsObject("TargetActor"))
		{
			ED = true;
			break;
		}
		if(UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - Cntrl->TimeStamp < MaxStimulusTime_Combat)
		{
			ED = true;
			break;
		}
	}
	return ED;
}

void AAIManager::RunCombatLoop()
{
	if(Engaged())
	{
		return;
	}
	NotifyAIState(EAIState::Idle);
	GetWorldTimerManager().ClearTimer(CombarTimer);
}

void AAIManager::RunSearchTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("ElapsedTime is: %f"), UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - TimerStamp));

	if(UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - TimerStamp < MaxStimulusTime_Search)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(SearchTimer);
	NotifyAIState(EAIState::Idle);
}

// might change here
void AAIManager::CreateAgentsList()
{
	TSubclassOf<ASRanged_AIController> classToFind = ASRanged_AIController::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, actors);

	for(auto & Cntrl : actors)
	{
		ASRanged_AIController* Cn = Cast<ASRanged_AIController>(Cntrl);
		if(Cn && Cn->Agent->faction == Faction)
		{
			Agents.AddUnique((Cn));
			Cn->AIManager = this;
		}
	}
}

void AAIManager::NotifyAIState(EAIState state)
{
	for( auto & Cntrl: Agents)
	{
		Cntrl->BBC->SetValueAsEnum("AIState", (uint8_t)state);
	}
	if(state == EAIState::Attack)
	{
		GetWorldTimerManager().ClearTimer(SearchTimer);
		GetWorldTimerManager().SetTimer(CombarTimer, this, &AAIManager::RunCombatLoop, 1.0f, true);
		return;
	}
	if(state == EAIState::Search)
	{
		TimerStamp = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		GetWorldTimerManager().SetTimer(SearchTimer, this, &AAIManager::RunSearchTimer, 1.0f, true);
		return;
	}
}

void AAIManager::RemoveAgent(ASRanged_AIController* controllerToRemove)
{
	int32 index = Agents.Find(controllerToRemove);
	if(index < 0)
	{
		return;
	}
	Agents.RemoveSingle(controllerToRemove);
}

