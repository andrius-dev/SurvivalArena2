#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "BasePlayerController.h"
#include "PlayerCharacterInterface.h"
#include "GameFramework/Character.h"
#include "BasePlayerCharacter.generated.h"

UCLASS(Blueprintable)
class TOPDOWN2_API ABasePlayerCharacter : public ACharacter, public IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	ABasePlayerCharacter();
	const float DEFAULT_MOVEMENT_DELTA_ANGLE = 45.f;
	
	// Higher means rotation will be more abrubt.
	UPROPERTY(EditAnywhere)
	float RotationEase = 10.f;
	
	// Max speed of rotation
	UPROPERTY(EditAnywhere)
	float RotationMaxSpeed = 25.f;
	
	UPROPERTY(EditAnywhere)
	float MovementDeltaAngle = DEFAULT_MOVEMENT_DELTA_ANGLE;
	
	virtual ACharacter* GetCharacter_Implementation() override;
	virtual UCombatComponent* GetCombatComponent_Implementation() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const {
		return TopDownCameraComponent;
	}

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {
		return CameraBoom;
	}
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void AddMovement(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void MouseLook(const FVector& Value, const float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	static float CalculateAngleFromGamepadInput(const FVector& GamepadInput);

	/**
	 * used to rotate character's camera around roll axis
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	double CameraRotationDelta;
	
	double DeltaTimeSecs;
	
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent = nullptr;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	
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

	UPROPERTY()
	TObjectPtr<ABasePlayerController> PlayerController = nullptr;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;

	UPROPERTY()
	FRotator3d CameraPositiveRotator;

	UPROPERTY()
	FRotator3d CameraNegativeRotator;
};
