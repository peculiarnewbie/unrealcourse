// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, int, CurrentCredits, int, ChangeCount);

UCLASS()
class UNREALCOURSE_API ABPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	int CreditsCount;
	
public:

	void AddCredits(int Count);

	bool SpendCredits(int Count);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

};
