// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPowerUpBase.h"
#include "BCoinPickUp.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOURSE_API ABCoinPickUp : public ABPowerUpBase
{
	GENERATED_BODY()

public:

	ABCoinPickUp();


protected:

	void ApplyPower(APawn* InstigatorPawn) override;
	int Value;
	
};
