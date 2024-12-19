#include "TopDown2/GAS/DamageExecutionCalculation.h"

struct FDamageStatics {
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;
	
	
	FDamageStatics() {}
}

void UDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const {
	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		