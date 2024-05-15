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

	//StaticMeshComp->OnComponentHit.AddDynamic(this, &ABExplosiveBarrel::Explode);

}

// Called when the game starts or when spawned
void ABExplosiveBarrel::BeginPlay()
{
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ABExplosiveBarrel::OnHit);
	Super::BeginPlay();
}

// Called every frame
void ABExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}

