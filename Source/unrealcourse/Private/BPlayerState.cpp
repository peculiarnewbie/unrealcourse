// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerState.h"
#include "BSaveGame.h"

void ABPlayerState::AddCredits(int Count)
{
	CreditsCount += Count;
	OnCreditsChanged.Broadcast(CreditsCount, Count);
}

bool ABPlayerState::SpendCredits(int Count)
{
	if (CreditsCount >= Count)
	{
		CreditsCount -= Count;
		OnCreditsChanged.Broadcast(CreditsCount, Count);
		return true;
	}
	return false;
}

int ABPlayerState::GetCredits() const
{
	return CreditsCount;
}

void ABPlayerState::SavePlayerState_Implementation(UBSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = CreditsCount;
	}
}

void ABPlayerState::LoadPlayerState_Implementation(UBSaveGame* SaveObject)
{
	if (SaveObject)
	{
		CreditsCount = SaveObject->Credits;
	}
}
