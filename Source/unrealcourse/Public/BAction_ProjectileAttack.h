// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BAction.h"
#include "BAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOURSE_API UBAction_ProjectileAttack : public UBAction
{
	GENERATED_BODY()



protected:

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	FRotator CalculateProjectileRotation(FVector start, FRotator rotation, FVector CamperaPosition, float endRange);

public:

	void StartAction_Implementation(AActor* Instigator) override;

	UBAction_ProjectileAttack();
	
};
