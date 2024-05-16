// Fill out your copyright notice in the Description page of Project Settings.


#include "BTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "BAttributeComponent.h"

// Sets default values
ABTargetDummy::ABTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UBAttributeComponent>("AttributeComp");

	AttributeComp->OnHealthChanged.AddDynamic(this, &ABTargetDummy::OnHealthChanged);

}

void ABTargetDummy::OnHealthChanged(AActor* InstigatorActor, UBAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}


