// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "SameFactionTest.generated.h"

/**
 * 
 */
UCLASS()
class UEnvQueryTest_FactionComparison : public UEnvQueryTest
{
	GENERATED_BODY()
	
	
public:
	UEnvQueryTest_FactionComparison(const FObjectInitializer& ObjectInitializer);
	
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};