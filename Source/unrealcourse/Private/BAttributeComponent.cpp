// Fill out your copyright notice in the Description page of Project Settings.


#include "BAttributeComponent.h"


// Sets default values for this component's properties
UBAttributeComponent::UBAttributeComponent()
{

	HealthMax = 100;
	Health = HealthMax;

}

bool UBAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UBAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	Health += Delta;
			
	OnHealthChanged.Broadcast(InstigatorActor, this, HealthMax, Health, Delta);

	if (Health < 0.0f) Health = 0.0f;
	else if (Health > HealthMax) Health = HealthMax;

	return true;
}

float UBAttributeComponent::GetIsHealthFull()
{
	return Health >= HealthMax;
}

UBAttributeComponent* UBAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UBAttributeComponent>(FromActor->GetComponentByClass(UBAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UBAttributeComponent::IsActorAlive(AActor* Actor)
{
	UBAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

