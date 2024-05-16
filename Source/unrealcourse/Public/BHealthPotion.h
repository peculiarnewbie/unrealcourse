// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPowerUpBase.h"
#include "BHealthPotion.generated.h"

UCLASS()
class UNREALCOURSE_API ABHealthPotion : public ABPowerUpBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABHealthPotion();

protected:

	virtual void ApplyPower(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly)
	float HealthValue;

};
