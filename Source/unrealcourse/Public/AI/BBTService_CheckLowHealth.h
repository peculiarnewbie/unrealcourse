// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOURSE_API UBBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
