// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOURSE_API UBBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Ai")
	float MaxBulletSpread;

	UPROPERTY(EditAnywhere, Category = "Ai")
	TSubclassOf<AActor> ProjectileClass;
	
};
