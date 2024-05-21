// Fill out your copyright notice in the Description page of Project Settings.


#include "BAction.h"

void UBAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void UBAction::StopAction_Implementation(AActor* Instigator)
{

	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

UWorld* UBAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
