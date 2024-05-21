// Fill out your copyright notice in the Description page of Project Settings.


#include "BHealthPotion.h"
#include "BAttributeComponent.h"
#include "BPlayerState.h"

// Sets default values
ABHealthPotion::ABHealthPotion()
{
	HealthValue = 50.0f;
	Cost = 5;
}

void ABHealthPotion::ApplyPower(APawn* InstigatorPawn)
{
	UBAttributeComponent* AttributeComp = InstigatorPawn->GetComponentByClass<UBAttributeComponent>();
	if (AttributeComp->GetIsHealthFull()) return;
	if (Cast<ABPlayerState>(InstigatorPawn->GetPlayerState())->SpendCredits(Cost))
	{
		AttributeComp->ApplyHealthChange(this, HealthValue);
		Deactivate();
	}
}


