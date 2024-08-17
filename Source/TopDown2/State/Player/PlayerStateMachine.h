#pragma once
#include "InputActionValue.h"
#include "PlayerState.h"
#include "TopDown2/State/IStateMachine.h"

class PlayerStateMachine : IStateMachine {
public:
	PlayerStateMachine();
	virtual void Tick(double Delta) override;
	void HandleMovementInput(
		UE::Math::TVector<double> ForwardDirection,
		UE::Math::TVector<double> RightDirection
	);
	void HandleMouseInput(const float DeltaTime, const FRotator& BodyRotator);
	void HandleAttackInput(const FInputActionValue& Value);

private:
	EPlayerState CurrentState;
};
