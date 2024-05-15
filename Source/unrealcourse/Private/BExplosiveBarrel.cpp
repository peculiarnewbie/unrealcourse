// Fill out your copyright notice in the Description page of Project Settings.

#include "BExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABExplosiveBarrel::ABExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComp->SetupAttachment(StaticMeshComp);

}

// Called when the game starts or when spawned
void ABExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABExplosiveBarrel::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit!"));
	RadialForceComp->FireImpulse();
}

void ABExplosiveBarrel::PostInitializeComponents()
{
	// Don't forget to call parent function
	Super::PostInitializeComponents();
}


void ABExplosiveBarrel::OnActorHit()
{
	RadialForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));


}