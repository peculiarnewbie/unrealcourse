// Fill out your copyright notice in the Description page of Project Settings.


#include "BHealthPotion.h"
#include "BAttributeComponent.h"

// Sets default values
ABHealthPotion::ABHealthPotion()
{
	HealthValue = 50.0f;
}

void ABHealthPotion::ApplyPower(APawn* InstigatorPawn)
{
	UBAttributeComponent* AttributeComp = InstigatorPawn->GetComponentByClass<UBAttributeComponent>();
	if (AttributeComp->GetIsHealthFull()) return;
	AttributeComp->ApplyHealthChange(HealthValue);
	Deactivate();
}


