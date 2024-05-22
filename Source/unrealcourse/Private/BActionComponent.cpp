// Fill out your copyright notice in the Description page of Project Settings.


#include "BActionComponent.h"
#include "BAction.h"
#include "../unrealcourse.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_BOLT);

UBActionComponent::UBActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UBActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UBAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}

	
}


void UBActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBActionComponent::AddAction(TSubclassOf<UBAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UBAction* NewAction = NewObject<UBAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UBActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for (UBAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UBActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UBAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

