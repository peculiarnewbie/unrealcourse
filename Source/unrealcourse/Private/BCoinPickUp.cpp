// Fill out your copyright notice in the Description page of Project Settings.


#include "BCoinPickUp.h"
#include "BPlayerState.h"

ABCoinPickUp::ABCoinPickUp()
{
	Value = 3;
}

void ABCoinPickUp::ApplyPower(APawn* InstigatorPawn)
{
	Super::ApplyPower(InstigatorPawn);

	ABPlayerState* PlayerState = Cast<ABPlayerState>(InstigatorPawn->GetPlayerState());
	if (PlayerState)
	{
		PlayerState->AddCredits(Value);
	}
}
