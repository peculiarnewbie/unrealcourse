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
#include "AI/BAICharacter.h"
#include "BPlayerState.h"
#include "Math/UnrealMathUtility.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("bu.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ABGameModeBase::ABGameModeBase()
{
	SpawnBotsInterval = 2.0f;
	SpawnPowerUpsInterval = 5.0f;

}

void ABGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ABGameModeBase::SpawnBotTimerElapsed, SpawnBotsInterval, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowers, this, &ABGameModeBase::SpawnPowerTimerElapsed, SpawnPowerUpsInterval, true);
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
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ABGameModeBase::OnBotQueryCompleted);
	}
}

void ABGameModeBase::OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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

void ABGameModeBase::SpawnPowerTimerElapsed()
{	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowersQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ABGameModeBase::OnSpawnQueryCompleted);
	}

}

void ABGameModeBase::OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powers EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		int index = FMath::RandRange(0, 1);
		GetWorld()->SpawnActor<AActor>(PowerClasses[index], Locations[0], FRotator::ZeroRotator);
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
		return;
	}

	ABAICharacter* Enemy = Cast<ABAICharacter>(VictimActor);
	if (Enemy)
	{
		Player = Cast<ABCharacter>(Killer);
		if (Player)
		{
			ABPlayerState* PlayerState = Cast<ABPlayerState>(Player->GetPlayerState());
			PlayerState->AddCredits(1);
		}

	}

	

}


