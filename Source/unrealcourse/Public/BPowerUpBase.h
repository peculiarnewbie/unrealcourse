// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BGameplayInterface.h"
#include "BPowerUpBase.generated.h"

UCLASS()
class UNREALCOURSE_API ABPowerUpBase : public AActor,  public IBGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABPowerUpBase();

	void Activate();
	void Deactivate();

protected:
	UPROPERTY(EditDefaultsOnly)
	float CooldownTimer;
	bool IsAvailable;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	FTimerHandle PowerUpCooldown_TimerHandle;

	void Interact_Implementation(APawn* IntegratorPawn);
	virtual void ApplyPower(APawn* InstigatorPawn);

};
