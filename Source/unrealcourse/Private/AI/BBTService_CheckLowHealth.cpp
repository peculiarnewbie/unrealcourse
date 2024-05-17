// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BBTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{

	}
}
