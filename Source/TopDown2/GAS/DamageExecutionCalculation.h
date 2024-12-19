#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecutionCalculation.generated.h"

/**
 * Calculates damage taken
 */
UCLASS()
class TOPDOWN2_API UDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};
