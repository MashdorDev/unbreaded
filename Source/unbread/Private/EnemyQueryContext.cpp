// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyQueryContext.h"
#include "SRanged_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ASRanged_AIController* Cntrl = Cast<ASRanged_AIController>(Cast<AActor>(QueryInstance.Owner.Get())->GetInstigatorController());

	if(Cntrl)
	{
		AActor* Target = Cast<AActor>(Cntrl->BBC->GetValueAsObject("TargetActor"));
		if(Target)
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Target);
		}
	}
}
