// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerState.h"

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
