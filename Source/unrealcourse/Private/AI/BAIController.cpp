// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}


//	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
//	if (PlayerPawn)
//	{
//		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
//		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
//	}
}
