// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BAttributeComponent.h"
#include "BCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UBInteractionComponent;

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void PrimaryAttack();
	void SpecialAttack();
	void PrimaryInteract();
	float teleportTime;
	AActor* DashActor;
	void Dash();
	void Teleport();

	FRotator CalculateProjectileRotation(FVector start, FRotator rotation, float endRange);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
