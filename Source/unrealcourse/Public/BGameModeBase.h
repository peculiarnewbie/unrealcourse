// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class UNREALCOURSE_API ABGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotsInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	UEnvQuery* SpawnPowersQuery;

	FTimerHandle TimerHandle_SpawnPowers;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	float SpawnPowerUpsInterval;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	TArray<TSubclassOf<AActor>> PowerClasses;

	UFUNCTION()
	void SpawnPowerTimerElapsed();

	UFUNCTION()
	void OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);


public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	ABGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();
	
};
