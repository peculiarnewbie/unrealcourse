// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class UNREALCOURSE_API ABAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);


public:	


};
