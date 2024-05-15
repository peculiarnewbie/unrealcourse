// Fill out your copyright notice in the Description page of Project Settings.


#include "BAttributeComponent.h"

// Sets default values for this component's properties
UBAttributeComponent::UBAttributeComponent()
{

	Health = 100;

}

bool UBAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

