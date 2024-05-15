// Fill out your copyright notice in the Description page of Project Settings.


#include "BItemChest.h"


// Sets default values
ABItemChest::ABItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

}


void ABItemChest::Interact_Implementation(APawn* IntegratorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

// Called when the game starts or when spawned
void ABItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

