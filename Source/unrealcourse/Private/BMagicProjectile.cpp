// Fill out your copyright notice in the Description page of Project Settings.


#include "BMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BAttributeComponent.h"

// Sets default values
ABMagicProjectile::ABMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABMagicProjectile::OnActorOverlap);

	Damage = 10.0f;
}


void ABMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UBAttributeComponent* AttributeComp = Cast<UBAttributeComponent>(OtherActor->GetComponentByClass(UBAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-Damage);

			if (ImpactSound != nullptr)
			{
				// UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
			}

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

