// Fill out your copyright notice in the Description page of Project Settings.


#include "BProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"


// Sets default values
ABProjectileBase::ABProjectileBase()
{

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	FlightAudio = CreateDefaultSubobject<UAudioComponent>("AudioComp");


}

// Called when the game starts or when spawned
void ABProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

