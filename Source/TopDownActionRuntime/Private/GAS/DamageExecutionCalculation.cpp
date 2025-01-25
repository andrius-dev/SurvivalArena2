#include "GAS/DamageExecutionCalculation.h"

#include "CombatAttributeSet.h"

struct FDamageStatics {
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;
	
	FDamageStatics() {
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(
			UCombatAttributeSet::GetBaseAttackAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			true // inSnapshot
		);
	}
};

// todo clean-up

UDamageExecutionCalculation::UDamageExecutionCalculation() {
	RelevantAttributesToCapture.Add(GetDamageStatics().BaseDamageDef);
}

FDamageStatics& UDamageExecutionCalculation::GetDamageStatics() {
	static FDamageStatics DamageStatics;
	return DamageStatics;
}

void UDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const {
	float Damage = 0.0f;
}
