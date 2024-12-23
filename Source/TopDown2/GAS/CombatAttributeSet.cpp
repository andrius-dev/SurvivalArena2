
#include "TopDown2/GAS/CombatAttributeSet.h"

UCombatAttributeSet::UCombatAttributeSet() {
}

bool UCombatAttributeSet::PreGameplayEffectExecute(
	FGameplayEffectModCallbackData& Data
) {
	return Super::PreGameplayEffectExecute(Data);
}

void UCombatAttributeSet::PostGameplayEffectExecute(
	const FGameplayEffectModCallbackData& Data
) {
	Super::PostGameplayEffectExecute(Data);
}

void UCombatAttributeSet::PreAttributeChange(
	const FGameplayAttribute& Attribute,
	float& NewValue
) {
	Super::PreAttributeChange(Attribute, NewValue);
}

void UCombatAttributeSet::PostAttributeChange(
	const FGameplayAttribute& Attribute,
	float OldValue,
	float NewValue
) {
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UCombatAttributeSet::InitFromMetaDataTable(const UDataTable* DataTable) {
	Super::InitFromMetaDataTable(DataTable);
}

void UCombatAttributeSet::ResetHealth() {
	SetHealth(GetMaxHealth());
}
