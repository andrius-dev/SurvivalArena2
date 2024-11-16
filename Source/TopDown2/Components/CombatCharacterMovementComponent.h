#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN2_API UCombatCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	bool DoDodge();
};
