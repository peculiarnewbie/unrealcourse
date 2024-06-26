// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BProjectileBase.h"
#include "BMagicProjectile.generated.h"


UCLASS()
class UNREALCOURSE_API ABMagicProjectile : public ABProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMagicProjectile();

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Called when the game starts or when spawned

	UPROPERTY(EditDefaultsOnly)
	float Damage;


};
