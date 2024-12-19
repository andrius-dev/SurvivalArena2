#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "BasePlayerController.h"
#include "PlayerCharacterInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "BasePlayerCharacter.generated.h"

UCLASS(Blueprintable)
class TOPDOWN2_API ABasePlayerCharacter : public ACharacter, public IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	ABasePlayerCharacter();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual ACharacter* GetCharacter_Implementation() override;
	
	virtual UCombatComponent* GetCombatComponent_Implementation() override;
	
	virtual void PostInitializeComponents() override;

protected:
	// Max speed of rotation
	UPROPERTY(EditAnywhere)
	float RotationMaxSpeed;
	
	UPROPERTY(EditAnywhere)
	float MovementDeltaAngle;

	UFUNCTION(BlueprintCallable, Category=AbilitySystem)
	void InitAttributes();
	
	/**
	 * used to rotate character's camera around roll axis
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	double CameraRotationDelta;

public:

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;
	
private:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AbilitySystem, meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	
	UPROPERTY()
	TObjectPtr<ABasePlayerController> PlayerController = nullptr;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;

	// Input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MovementInputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MeleeAttackInputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GunAttackInputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MouseLookInputAction = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ControllerLookInputAction = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpInputAction = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DodgeInputAction = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PanCameraLeftInputAction = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PanCameraRightInputAction = nullptr;

	// Camera rotators
	UPROPERTY()
	FRotator3d CameraPositiveRotator;

	UPROPERTY()
	FRotator3d CameraNegativeRotator;
	
	// Higher means rotation will be more abrubt.
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess  = "true"))
	float RotationEase;

	UPROPERTY()
	double DeltaTimeSecs;
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void AddMovement(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void MouseLook(const FVector& Value, const float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	static float CalculateAngleFromGamepadInput(const FVector& GamepadInput);
};
