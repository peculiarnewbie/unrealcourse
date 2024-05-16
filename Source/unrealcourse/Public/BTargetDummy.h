// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTargetDummy.generated.h"

class UBAttributeComponent;

UCLASS()
class UNREALCOURSE_API ABTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UBAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UBAttributeComponent* OwningComp, float HealthMax, float NewHealth, float Delta);

};
