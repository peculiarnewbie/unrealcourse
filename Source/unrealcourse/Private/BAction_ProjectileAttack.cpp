// Fill out your copyright notice in the Description page of Project Settings.


#include "BAction_ProjectileAttack.h"
#include "GameFramework/Character.h"

UBAction_ProjectileAttack::UBAction_ProjectileAttack()
{
}

void UBAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
}

void UBAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		FVector HandLocation = Character->GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator projectileRotation = CalculateProjectileRotation(HandLocation, Character->GetControlRotation(), Character->GetPawnViewLocation(), 10000);

		FTransform SpawnTM = FTransform(projectileRotation, HandLocation);
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Character;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);;
	}
}

FRotator UBAction_ProjectileAttack::CalculateProjectileRotation(FVector start, FRotator rotation, FVector CameraPosition, float endRange)
{
	FRotator endRotation;


	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_MAX); // probably shouldn't do this 

	FVector End = CameraPosition + (rotation.Vector() * endRange);

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraPosition, End, ObjectQueryParams);


	if (bBlockingHit)
	{
		endRotation = ( Hit.ImpactPoint - start).Rotation();
	}
	else
	{
		endRotation = (Hit.TraceEnd - start).Rotation();
	}

//	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
//	DrawDebugLine(GetWorld(), CameraPosition, End, LineColor, false, 2.0f, 0, 2.0f);

	return endRotation;
}

