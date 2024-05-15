// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BExplosiveBarrel.generated.h"

class URadialForceComponent;
class UStaticMeshComponent;

UCLASS()
class UNREALCOURSE_API ABExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	// Called when the game starts or when spawned

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
