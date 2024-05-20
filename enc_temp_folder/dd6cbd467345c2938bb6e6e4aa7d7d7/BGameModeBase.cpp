// Fill out your copyright notice in the Description page of Project Settings.


#include "BGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/BAICharacter.h"
#include "BAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "BCharacter.h"


ABGameModeBase::ABGameModeBase()
{
	SpawnTimerInterval = 2.0f;

}

void ABGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ABGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ABGameModeBase::KillAll()
{

	for (TActorIterator<ABAICharacter> It(GetWorld()); It; ++It)
	{
		ABAICharacter* Bot = *It;
		UBAttributeComponent* AttributeComp = UBAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); //FIXME: pass in player

		}
	}
}

void ABGameModeBase::SpawnBotTimerElapsed()
{

	int32 NrOfAliveBots = 0;
	for (TActorIterator<ABAICharacter> It(GetWorld()); It; ++It)
	{
		ABAICharacter* Bot = *It;
		UBAttributeComponent* AttributeComp = UBAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ABGameModeBase::OnQueryCompleted);
	}
}

void ABGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ABGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ABGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ABCharacter* Player = Cast<ABCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
}


