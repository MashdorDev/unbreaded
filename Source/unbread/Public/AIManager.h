// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "unbread/unbread.h"
#include "AIManager.generated.h"

UCLASS()
class UNBREAD_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

	UPROPERTY(BlueprintReadOnly)
	TArray<class ASRanged_AIController*> Agents;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	EFaction Faction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float MaxStimulusTime_Combat = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float MaxStimulusTime_Search = 10.0f;

	UFUNCTION()
	bool Engaged();

	FTimerHandle CombarTimer;
	FTimerHandle SearchTimer;

	UFUNCTION()
	void RunCombatLoop();

	float TimerStamp = 0.0f;

	UFUNCTION()
	void RunSearchTimer();

public:
	UFUNCTION()
	void CreateAgentsList();

	UFUNCTION()
	void NotifyAIState(EAIState state);

	UFUNCTION()
	void SwitchFaction(ASRanged_AIController* agent);

	UFUNCTION()
	void RemoveAgent(ASRanged_AIController* controllerToRemove);
private:
	AAIManager* OppositeFaction;
};
