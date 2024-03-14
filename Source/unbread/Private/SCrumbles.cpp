// Fill out your copyright notice in the Description page of Project Settings.


#include "SCrumbles.h"

#include "CookieInterface.h"
#include "Components/BoxComponent.h"

// Sets default values
ASCrumbles::ASCrumbles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("TriggerVolume");
	TriggerVolume->SetupAttachment(Mesh);
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ASCrumbles::OnEnterVolume);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ASCrumbles::OnExitVolume);

}

// Called when the game starts or when spawned
void ASCrumbles::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCrumbles::OnEnterVolume(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->ImplementsInterface(UCookieInterface::StaticClass()))
	{
		ICookieInterface::Execute_SetNearestCrumblePile(OtherActor, this);
	}
}

void ASCrumbles::OnExitVolume(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->ImplementsInterface(UCookieInterface::StaticClass()))
	{
		ICookieInterface::Execute_SetNearestCrumblePile(OtherActor, nullptr);
	}
}

// Called every frame
void ASCrumbles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCrumbles::CrumbleInteraction_Implementation(AActor* InstigatingActor)
{
	IInteractInterface::CrumbleInteraction_Implementation(InstigatingActor);
	if(bDestroyOnUse)
	{
		Destroy();
	}
}

