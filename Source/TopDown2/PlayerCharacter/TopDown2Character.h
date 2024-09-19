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
	
	virtual void Tick(float DeltaSeconds) override;
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UFUNCTION(BlueprintCallable, Category=Animation)
	void StartHitDetection();
	// Higher means rotation will be more abrubt.
	UPROPERTY(EditAnywhere)
	float RotationEase = 10.f;
	// Max speed of rotation
	UPROPERTY(EditAnywhere)
	float RotationMaxSpeed = 25.f;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	void Move(const FInputActionValue& Value);
	void MeleeAttack(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value, const float DeltaTime);
	static float CalculateAngleFromGamepadInput(const FVector& GamepadInput);
	double DeltaTimeSecs;

private:
	int CameraYawAngle = 0;
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MeleeAttackInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* GunAttackInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MouseLookInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ControllerLookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation, meta=(AllowPrivateAccess = "true"))
	UAnimMontage* MeleeAttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Controller, meta=(AllowPrivateAccess = "true"))
	ATopDown2PlayerController* PlayerController;
};
