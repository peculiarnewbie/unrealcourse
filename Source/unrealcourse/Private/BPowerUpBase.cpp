// Fill out your copyright notice in the Description page of Project Settings.


#include "BPowerUpBase.h"

// Sets default values
ABPowerUpBase::ABPowerUpBase()
{
	IsAvailable = true;
	CooldownTimer = 10.0f;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}
	
void ABPowerUpBase::Interact_Implementation(APawn* IntegratorPawn)
{
	if (!IsAvailable) return;
	ApplyPower(IntegratorPawn);
}

void ABPowerUpBase::ApplyPower(APawn* IntegratorPawn)
{
	Deactivate();
}

void ABPowerUpBase::Deactivate()
{
	IsAvailable = false;
	BaseMesh->SetVisibility(false);
	GetWorldTimerManager().SetTimer(PowerUpCooldown_TimerHandle, this, &ABPowerUpBase::Activate, CooldownTimer);
}

void ABPowerUpBase::Activate()
{
	IsAvailable = true;
	BaseMesh->SetVisibility(true);
}


