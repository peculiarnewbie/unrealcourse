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
#include "BSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "BGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("bu.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ABGameModeBase::ABGameModeBase()
{
	SpawnBotsInterval = 2.0f;
	SpawnPowerUpsInterval = 5.0f;

	SlotName = "SaveGame01";

}

void ABGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

void ABGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ABGameModeBase::SpawnBotTimerElapsed, SpawnBotsInterval, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowers, this, &ABGameModeBase::SpawnPowerTimerElapsed, SpawnPowerUpsInterval, true);

	LoadSaveGame();
}

void ABGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ABPlayerState* PS = NewPlayer->GetPlayerState<ABPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
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

void ABGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ABPlayerState* PS = Cast<ABPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
		}
	}

	CurrentSaveGame->SaveActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UBGameplayInterface>())
		{
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("Adding actor to saveactors"));

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;

		Actor->Serialize(Ar);

		CurrentSaveGame->SaveActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ABGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UBSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data."));
			return;
		}

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			
			if (!Actor->Implements<UBGameplayInterface>())
			{
				continue;
			}
	
	
			for (FActorSaveData ActorData : CurrentSaveGame->SaveActors)
			{

				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;

					Actor->Serialize(Ar);

					IBGameplayInterface::Execute_OnActorLoaded(Actor);

					
					break;
				}
			}
		}
	}

	else
	{
		CurrentSaveGame = Cast<UBSaveGame>(UGameplayStatics::CreateSaveGameObject(UBSaveGame::StaticClass()));
		
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame data."));
	}



}


