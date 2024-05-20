// Fill out your copyright notice in the Description page of Project Settings.

#include "BCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BInteractionComponent.h"
#include "BAttributeComponent.h"


	

// Sets default values
ABCharacter::ABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmCom");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UBInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UBAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	teleportTime = 0.4f;
}

void ABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ABCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();

	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);
}

void ABCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();

	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

// Called every frame
void ABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ABCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ABCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &ABCharacter::SpecialAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABCharacter::Dash);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ABCharacter::PrimaryInteract);
}

void ABCharacter::HealSelf(float Amount /* = 100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ABCharacter:: PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator projectileRotation = CalculateProjectileRotation(HandLocation, GetControlRotation(), 10000);

	FTransform SpawnTM = FTransform(projectileRotation, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);;
}

void ABCharacter::SpecialAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_02");
	FRotator projectileRotation = CalculateProjectileRotation(HandLocation, GetControlRotation(), 3000);

	FTransform SpawnTM = FTransform(projectileRotation, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(SpecialProjectileClass, SpawnTM, SpawnParams);;
}

void ABCharacter::Dash()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_02");
	FRotator projectileRotation = CalculateProjectileRotation(HandLocation, GetControlRotation(), 3000);

	FTransform SpawnTM = FTransform(projectileRotation, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	DashActor = GetWorld()->SpawnActor<AActor>(DashProjectileClass, SpawnTM, SpawnParams);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABCharacter::Teleport, teleportTime, false);
}

void ABCharacter::Teleport()
{
	FVector newPosition = DashActor->GetActorLocation();
	SetActorLocation(newPosition);
	return;
}

FRotator ABCharacter::CalculateProjectileRotation(FVector start, FRotator rotation, float endRange)
{
	FRotator endRotation;

	AActor* MyOwner = CameraComp->GetOwner();

	FVector CameraPosition = CameraComp->GetComponentLocation();


	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_MAX); // probably shouldn't do this 

	FVector End = CameraPosition + (rotation.Vector() * endRange);

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraPosition, End, ObjectQueryParams);


	if (bBlockingHit)
	{
		endRotation = ( Hit.ImpactPoint - start).Rotation();
	}
	else
	{
		endRotation = (Hit.TraceEnd - start).Rotation();
	}

//	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
//	DrawDebugLine(GetWorld(), CameraPosition, End, LineColor, false, 2.0f, 0, 2.0f);

	return endRotation;
}

void ABCharacter::PrimaryInteract() 
{
	InteractionComp->PrimaryInteract();
}


void ABCharacter::OnHealthChanged(AActor* InstigatorActor, UBAttributeComponent* OwningComp,float HealthMax, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

	}
	if (Delta < 0.0f)
	{
		GetComponentByClass<USkeletalMeshComponent>()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
