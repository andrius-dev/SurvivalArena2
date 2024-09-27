#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "TopDown2PlayerController.h"
#include "GameFramework/Character.h"
#include "TopDown2Character.generated.h"

UCLASS(Blueprintable)
class ATopDown2Character : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDown2Character();

	const float DEFAULT_MOVEMENT_DELTA_ANGLE = 45.f;
	
	// Higher means rotation will be more abrubt.
	UPROPERTY(EditAnywhere)
	float RotationEase = 10.f;
	
	// Max speed of rotation
	UPROPERTY(EditAnywhere)
	float RotationMaxSpeed = 25.f;
	
	UPROPERTY(EditAnywhere)
	float MovementDeltaAngle = DEFAULT_MOVEMENT_DELTA_ANGLE;

	virtual void Tick(float DeltaSeconds) override;
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const {
		return TopDownCameraComponent;
	}

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {
		return CameraBoom;
	}

	UFUNCTION(BlueprintCallable, Category=Animation)
	void StartHitDetection();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	void AddMovement(const FInputActionValue& Value);
	void StopMovement();
	void MeleeAttack(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value, const float DeltaTime);
	static float CalculateAngleFromGamepadInput(const FVector& GamepadInput);
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MeleeAttackAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Controller, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<ATopDown2PlayerController> PlayerController = nullptr;
};
