
#include "TopDown2/GAS/CombatCharacterAttributeSet.h"

UCombatCharacterAttributeSet::UCombatCharacterAttributeSet() {
}

bool UCombatCharacterAttributeSet::PreGameplayEffectExecute(
	FGameplayEffectModCallbackData& Data
) {
	return Super::PreGameplayEffectExecute(Data);
}

void UCombatCharacterAttributeSet::PostGameplayEffectExecute(
	const FGameplayEffectModCallbackData& Data
) {
	Super::PostGameplayEffectExecute(Data);
}

void UCombatCharacterAttributeSet::PreAttributeChange(
	const FGameplayAttribute& Attribute,
	float& NewValue
) {
	Super::PreAttributeChange(Attribute, NewValue);
}

void UCombatCharacterAttributeSet::PostAttributeChange(
	const FGameplayAttribute& Attribute,
	float OldValue,
	float NewValue
) {
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UCombatCharacterAttributeSet::InitFromMetaDataTable(const UDataTable* DataTable) {
	Super::InitFromMetaDataTable(DataTable);
}
