// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown2Character.h"

#include "EnhancedInputComponent.h"
#include "TopDown2PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ATopDown2Character::ATopDown2Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATopDown2Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ATopDown2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (auto EInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EInputComponent->BindAction(
			MovementInputAction,
			ETriggerEvent::Triggered,
			this,
			&ATopDown2Character::Move
		);
	} else {
		UE_LOG(
			LogTemplateCharacter,
			Error,
			TEXT(
				"'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
			),
			*GetNameSafe(this)
		);
	}
}


void ATopDown2Character::Move(const FInputActionValue& Value) {
	if (Controller == nullptr) {
		UE_LOG(LogTemplateCharacter, Error, TEXT("bybis"), *GetNameSafe(this));
		return;
	}
	auto MovementVector = Value.Get<FVector>();
	const FRotator YawRotation(0, 0, 0);
	const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}
