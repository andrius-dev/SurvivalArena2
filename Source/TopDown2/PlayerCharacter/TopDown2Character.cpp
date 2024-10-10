#include "TopDown2Character.h"

#include "EnhancedInputComponent.h"
#include "TopDown2PlayerController.h"
#include "AI/NavigationSystemBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TopDown2/Constants.h"

// todo: clean this up, most of these params should be set in blueprint
ATopDown2Character::ATopDown2Character() {
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	DeltaTimeSecs = 0.0;
	PrimaryActorTick.bCanEverTick = true;

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
	MovementDeltaAngle = CameraBoom->GetComponentTransform().GetRotation().Z;
}

void ATopDown2Character::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	DeltaTimeSecs = DeltaSeconds;
}

bool ATopDown2Character::IsDodgePressed_Implementation() {
	return bDodgePressed;
}

bool ATopDown2Character::IsAttackPressed_Implementation() {
	return bAttackPressed;
}

int ATopDown2Character::AttackInputCount_Implementation() {
	return AttackInputCount;
}

void ATopDown2Character::ResetAttackInputChain_Implementation() {
	AttackInputCount = 0;
}

void ATopDown2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) {
		UE_LOG(
			LogTopDown2,
			Error,
			TEXT(
				"'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
			),
			*GetNameSafe(this)
		);
		return;
	}
	EnhancedInputComponent->BindActionValueLambda(
		MovementInputAction,
		ETriggerEvent::Triggered,
		[this](const FInputActionValue& Value) {
			if (!GetCharacterMovement()->IsFalling()) {
				AddMovement(Value);	
			}	
		}	
	);
	EnhancedInputComponent->BindAction(
		MovementInputAction,
		ETriggerEvent::Completed,
		this,
		&ATopDown2Character::StopMovement
	);
	EnhancedInputComponent->BindAction(
		MeleeAttackInputAction,
		ETriggerEvent::Triggered,
		this,
		&ATopDown2Character::MeleeAttack
	);
	EnhancedInputComponent->BindActionValueLambda(
		MouseLookInputAction,
		ETriggerEvent::None,
		[this](const FInputActionValue& Value) {
			if (GetCharacterMovement()->IsFalling()) {
				MouseLook(Value.Get<FVector>(), DeltaTimeSecs);
			}
		}
	);
	EnhancedInputComponent->BindActionValueLambda(
		JumpInputAction,
		ETriggerEvent::Triggered,
		[this](const FInputActionValue& Value) {
			bIsJumpPressed = Value.Get<bool>();
			Jump();
		}	
	);
	EnhancedInputComponent->BindActionValueLambda(
		JumpInputAction,
		ETriggerEvent::Canceled,
		[this](const FInputActionValue& Value) {
			bIsJumpPressed = Value.Get<bool>();
		}	
	);
	EnhancedInputComponent->BindActionValueLambda(
		DodgeInputAction,
		ETriggerEvent::Triggered,
		[this](const FInputActionValue& Value) {
			bDodgePressed = true;
		}
	);
	EnhancedInputComponent->BindActionValueLambda(
		DodgeInputAction,
		ETriggerEvent::Canceled,
		[this](const FInputActionValue& Value) {
			bDodgePressed = false;
		}
	);
}

void ATopDown2Character::Dodge(const FInputActionValue& Value) {
	bDodgePressed = Value.Get<bool>();
}

void ATopDown2Character::PossessedBy(AController* NewController) {
	PlayerController = Cast<ATopDown2PlayerController>(NewController);
	if (!PlayerController || !PlayerController->IsLocalController()) {
		return;
	}
	PlayerController->SetShowMouseCursor(true); // true
	PlayerController->bEnableMouseOverEvents = true; // true
	PlayerController->SetInputMode(FInputModeGameOnly());
	
}

void ATopDown2Character::AddMovement(const FInputActionValue& Value) {
	if (Controller == nullptr) {
		UE_LOG(LogTopDown2, Error, TEXT("Controller is null"));
		return;
	}
	
	if (!Value.IsNonZero()) {
		UE_LOG(LogTopDown2, Error, TEXT("Value is zero"));	
		return;
	}
	
	const auto GamepadInput = Value.Get<FVector>();
	
	const float Angle = CalculateAngleFromGamepadInput(GamepadInput);
	const FRotator BodyRotator = FRotator(0.f, Angle + MovementDeltaAngle, 0.f);
    SetActorRotation(BodyRotator);

	const auto ForwardDirection = 
		FRotationMatrix(FRotator::ZeroRotator).GetUnitAxis(EAxis::X);
	const auto RightDirection =
		FRotationMatrix(FRotator::ZeroRotator).GetUnitAxis(EAxis::Y);
	const auto UpDirection =
		FRotationMatrix(FRotator::ZeroRotator).GetUnitAxis(EAxis::Z);
	
	const auto OffsetInput = GamepadInput.RotateAngleAxis(-MovementDeltaAngle, UpDirection);
	
	AddMovementInput(ForwardDirection, OffsetInput.Y);
	AddMovementInput(RightDirection, OffsetInput.X);
}

void ATopDown2Character::StopMovement() {
	AddMovement(FInputActionValue());
}

// todo combat component
void ATopDown2Character::MeleeAttack(const FInputActionValue& Value) {
	if (!MeleeAttackAnimMontage) {
		return;	
	}
	PlayAnimMontage(MeleeAttackAnimMontage);
	auto AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(MeleeAttackAnimMontage);
	auto name = MeleeAttackAnimMontage->GetName();
	UE_LOG(LogTopDown2, All, TEXT("%s"), *name);
}

void ATopDown2Character::MouseLook(const FVector& Value, const float DeltaTime) {
	FVector MouseDir = FVector::ZeroVector;
	FVector MousePos = FVector::ZeroVector;
	if (Value.IsZero()) {
	
		PlayerController->DeprojectMousePositionToWorld(MousePos, MouseDir);
	} else {
		const auto ScreenPos = FVector2D(Value.X, Value.Y);
		UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenPos, MousePos, MouseDir);
	}

	// Declaration of vector of intersection.
	FVector Intersection = FVector::ZeroVector;
	float t = 0.f;
	// Vector from camera that crosses the plane we want the intersection.
	FVector LineEnd = MousePos + MouseDir * 3000.f;
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
	if (!bIntersectionSuccess) {
		// Debug
		return;
	}
	
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
	const FRotator BodyRotator = FRotator(0.f, Angle * DeltaTime, 0.f);
	// Add rotation to pawn body component.
	AddActorLocalRotation(BodyRotator);
	
	// Debug
	DrawDebugLine(GetWorld(), GetActorLocation(), Intersection, FColor::Orange, false, -1.f, 0, 4.f);
	DrawDebugSphere(GetWorld(), Intersection, 10.f, 16, FColor::Red, false);
}

float ATopDown2Character::CalculateAngleFromGamepadInput(const FVector& GamepadInput) {
	return atan2(GamepadInput.X, GamepadInput.Y) * Constants::RadToDeg;
}
