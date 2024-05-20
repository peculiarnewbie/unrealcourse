// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BAttributeComponent.h"
#include "BrainComponent.h"
#include "UI/BWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABAICharacter::ABAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<UBAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}


void ABAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ABAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ABAICharacter::OnHealthChanged);
}

void ABAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}

}

void ABAICharacter::OnPawnSeen(APawn* Pawn)
{

	SetTargetActor(Pawn);
	
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ABAICharacter::OnHealthChanged(AActor* InstigatorActor, UBAttributeComponent* OwningComp, float HealthMax, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{

		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{

			ActiveHealthBar = CreateWidget<UBWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		
		}


		if (NewHealth <= 0.0f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set lifespan
			SetLifeSpan(10.0f);

		}

		GetComponentByClass<USkeletalMeshComponent>()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

}


