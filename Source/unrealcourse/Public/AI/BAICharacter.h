// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BAttributeComponent.h"
#include "BAICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AActor*, InstigatorActor, UBAttributeComponent*, OwningComp);

class UPawnSensingComponent;
class UBAttrbuteComponent;
class UUserWidget;
class UBWorldUserWidget;

UCLASS()
class UNREALCOURSE_API ABAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABAICharacter();

protected:

	UBWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UBAttributeComponent* OwningComp, float HealthMax, float NewHealth, float Delta);

public:	

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

};
