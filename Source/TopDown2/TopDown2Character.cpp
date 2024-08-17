// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown2Character.h"

#include "EnhancedInputComponent.h"
#include "GameTags.h"
#include "KismetTraceUtils.h"
#include "TopDown2PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Util/ColorConstants.h"

ATopDown2Character::ATopDown2Character() {
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
	Tags.Add(FName(GameTags::Player));
	
}

void ATopDown2Character::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	MouseLook(GetMesh(), DeltaSeconds);
}

void ATopDown2Character::StartHitDetection() {
	// if (RightHandSocketName == "") {
	// 	return;
	// }
	// auto socketLocation = GetMesh()->GetSocketLocation(RightHandSocketName);
	// FHitResult hitResult;
	// UKismetSystemLibrary::CapsuleTraceMultiForObjects(
	// 	
	// );
	// DrawDebugSphereTraceSingle(
	// 	GetWorld(),
	// 	socketLocation,
	// 	socketLocation,
	// 	50.f,
	// 	EDrawDebugTrace::ForDuration,
	// 	false,
	// 	hitResult,
	// 	FLinearColor::Yellow,
	// 	FLinearColor::Red,
	// 	4.f
	// 	);
	// 	UGameplayStatics::BreakHitResult(hitResult);
}

void ATopDown2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	auto inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!inputComponent) {
		UE_LOG(
			LogTemplateCharacter,
			Error,
			TEXT(
				"'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
			),
			*GetNameSafe(this)
		);
		return;
	}
	inputComponent->BindAction(
		MovementInputAction,
		ETriggerEvent::Triggered,
		this,
		&ATopDown2Character::Move
	);
	inputComponent->BindAction(
		MeleeAttackInputAction,
		ETriggerEvent::Triggered,
		this,
		&ATopDown2Character::MeleeAttack
	);
	inputComponent->BindAction(
		GunAttackInputAction,
		ETriggerEvent::Triggered,
		this,
		&ATopDown2Character::GunAttack
	);
}

void ATopDown2Character::PossessedBy(AController* NewController) {
	PlayerController = Cast<ATopDown2PlayerController>(NewController);
	if (!PlayerController || !PlayerController->IsLocalController()) {
		return;
	}
	PlayerController->SetShowMouseCursor(true);
	PlayerController->bEnableMouseOverEvents = true;
}

void ATopDown2Character::Move(const FInputActionValue& Value) {
	if (Controller == nullptr) {
		UE_LOG(LogTemplateCharacter, Error, TEXT("šikna"), *GetNameSafe(this));
		return;
	}
	const auto MovementVector = Value.Get<FVector>();
	const auto ComponentRotation = RootComponent->GetComponentRotation();
	const auto ForwardDirection = 
		FRotationMatrix(ComponentRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection =
		FRotationMatrix(ComponentRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ATopDown2Character::MeleeAttack(const FInputActionValue& Value) {
	float AnimDuration = PlayAnimMontage(MeleeAttackAnimMontage, 1);
	FString str = std::to_string(AnimDuration).c_str();
	UE_LOG(LogTemplateCharacter, Error, TEXT("Anim duration: %s"), *str);
}

void ATopDown2Character::GunAttack(const FInputActionValue& Value) {
}

void ATopDown2Character::MouseLook(USceneComponent* Comp1, float DeltaTime) {
	if (!PlayerController) {
		return;
	}
	FVector MouseDir = FVector::ZeroVector;
	FVector MousePos = FVector::ZeroVector;
	PlayerController->DeprojectMousePositionToWorld(MousePos, MouseDir);

	// Declaration of vector of intersection.
	FVector Intersection = FVector::ZeroVector;
	float t = 0.f;
	// Vector from camera that crosses the plane we want the intersection.
	FVector LineEnd = MousePos + MouseDir * 2000.f;
	// Get intersection vector. Returns true if intersection was possible.

	bool bIntersectionSuccess = UKismetMathLibrary::LinePlaneIntersection_OriginNormal(
		MousePos,
		LineEnd,
		GetActorLocation(),
		GetActorUpVector(),
		t,
		Intersection
	);
	// Do stuff if line intersected.
	if (bIntersectionSuccess) {
		// Calculate direction vector from the pawn body forward vector to intersection vector.
		FVector DirToIntersection = (Intersection - GetActorLocation()).GetSafeNormal();
		// Gets the cosine of the angle between the pawns body forward vector and the direction to intersection.
		float dotForward = GetActorForwardVector() | DirToIntersection;
		// Converts the cosine of the angle to degrees.
		float Angle = acos(dotForward) * (180.f / PI);
		// Clamp to limit how fast the component can rotate.
		Angle = FMath::Clamp(Angle, 0.f, RotationMaxSpeed);
		// Gets the cosine of the angle with the right vector against direction to intersection to know on what side of the component is the intersection.
		float dotSide = GetActorRightVector() | DirToIntersection;
		// Negates the value depending on what side is the intersection relative to the component.
		Angle *= RotationEase * ((dotSide > 0.f) ? 1.f : -1.f);
		// Create rotator with variable.
		FRotator BodyRotator = FRotator(0.f, Angle * DeltaTime, 0.f);
		// Add rotation to pawn body component.
		AddActorLocalRotation(BodyRotator);
	}
	// Debug
	DrawDebugLine(GetWorld(), GetActorLocation(), Intersection, FColor::Orange, false, -1.f, 0, 4.f);
	DrawDebugSphere(GetWorld(), Intersection, 10.f, 16, FColor::Red, false);
}
