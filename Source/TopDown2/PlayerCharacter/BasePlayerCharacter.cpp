#include "BasePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "BasePlayerController.h"
#include "Animation/AnimAttributes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TopDown2/Util/GameMaths.h"
#include "TopDown2/Util/Log.h"

ABasePlayerCharacter::ABasePlayerCharacter() {
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	DeltaTimeSecs = 0.0;
	PrimaryActorTick.bCanEverTick = true;

	// Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	RotationEase = 10.f;
	RotationMaxSpeed = 25.f;
	
	// Character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	MovementDeltaAngle = 45;

	// Camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Camera
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	MovementDeltaAngle = CameraBoom->GetComponentTransform().GetRotation().Z;
	MovementComponent = CastChecked<UCharacterMovementComponent>(ACharacter::GetMovementComponent());
	CameraRotationDelta = 45;
	CameraPositiveRotator = FRotator3d(0.0, -CameraRotationDelta, 0.0);
	CameraNegativeRotator = FRotator3d(0.0, CameraRotationDelta, 0.0);
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatAttributeSet = CreateDefaultSubobject<UAttributeSet>(TEXT("CombatAttributeSet"));
}

ACharacter* ABasePlayerCharacter::GetCharacter_Implementation() {
	return this;
}

UCombatComponent* ABasePlayerCharacter::GetCombatComponent_Implementation() {
	return CombatComponent;
}

void ABasePlayerCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	InitAttributes();
}

void ABasePlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	AbilitySystemComponent = FindComponentByClass<UAbilitySystemComponent>();
	InitAttributes();
}

void ABasePlayerCharacter::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	DeltaTimeSecs = DeltaSeconds;
}

UAbilitySystemComponent* ABasePlayerCharacter::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

void ABasePlayerCharacter::InitAttributes() {
	// if (IsValid(AbilitySystem.Get())) {
			
	// }
}

void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) {
		UE_LOG(
			LogTopDown2,
			Error,
			TEXT(
				"'%s' Failed to find an Enhanced Input Component..! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
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
	EnhancedInputComponent->BindActionValueLambda(
		MovementInputAction,
		ETriggerEvent::Canceled,
		[this](const FInputActionValue& Value) {
			if (!GetCharacterMovement()->IsFalling()) {
			}	
		}	
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
		PanCameraLeftInputAction,
		ETriggerEvent::Triggered,
		[this](const FInputActionValue& Value) {
			const auto NewRotation = CameraBoom->GetComponentRotation() +
				CameraNegativeRotator;
			CameraBoom->SetWorldRotation(NewRotation);
			MovementDeltaAngle = CameraBoom->GetComponentTransform().GetRotation().Z;
		}	
	);
	EnhancedInputComponent->BindActionValueLambda(
		PanCameraRightInputAction,
		ETriggerEvent::Triggered,
		[this](const FInputActionValue& Value) {
			const auto NewRotation = CameraBoom->GetComponentRotation() +
				CameraPositiveRotator;
			CameraBoom->SetWorldRotation(NewRotation);
			MovementDeltaAngle = CameraBoom->GetComponentTransform().GetRotation().Z;
		}	
	);

}

void ABasePlayerCharacter::PossessedBy(AController* NewController) {
	PlayerController = Cast<ABasePlayerController>(NewController);
	if (!PlayerController || !PlayerController->IsLocalController()) {
		return;
	}
	PlayerController->SetShowMouseCursor(true);
	PlayerController->bEnableMouseOverEvents = true; // true
	PlayerController->SetInputMode(FInputModeGameOnly());
	MovementDeltaAngle = CameraBoom->GetComponentTransform().GetRotation().Z;
}

void ABasePlayerCharacter::AddMovement(const FInputActionValue& Value) {
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
	
	const auto OffsetInput = GamepadInput.RotateAngleAxis(MovementDeltaAngle, UpDirection);
	
	AddMovementInput(ForwardDirection, OffsetInput.Y);
	AddMovementInput(RightDirection, OffsetInput.X);
}

void ABasePlayerCharacter::MouseLook(const FVector& Value, const float DeltaTime) {
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

float ABasePlayerCharacter::CalculateAngleFromGamepadInput(const FVector& GamepadInput) {
	return atan2(GamepadInput.X, GamepadInput.Y) * GameMaths::RadToDeg;
}
