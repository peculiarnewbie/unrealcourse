// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UBInteractionComponent;
class UBActionComponent;

UCLASS()
class UNREALCOURSE_API ABCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpecialProjectileClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DashProjectileClass;

public:
	// Sets default values for this character's properties
	ABCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UBInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBActionComponent* ActionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void SprintStart();
	void SprintStop();

	void PrimaryAttack();
	void SpecialAttack();
	void PrimaryInteract();
	void Dash();

	void SaveGame();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UBAttributeComponent* OwningComp, float HealthMax, float NewHealth, float Delta);

	virtual void PostInitializeComponents();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	virtual FVector GetPawnViewLocation() const override;

	void Teleport(AActor* DashActor);


};
