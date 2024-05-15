// Fill out your copyright notice in the Description page of Project Settings.


#include "BMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BAttributeComponent.h"

// Sets default values
ABMagicProjectile::ABMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABMagicProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

}


void ABMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UBAttributeComponent* AttributeComp = Cast<UBAttributeComponent>(OtherActor->GetComponentByClass(UBAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}

}

// Called when the game starts or when spawned
void ABMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

