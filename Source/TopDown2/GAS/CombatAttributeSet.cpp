#include "TopDown2/GAS/CombatAttributeSet.h"
#include "TopDown2/Util/Log.h"

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
	const float OldValue,
	const float NewValue
) {
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	LogAttributeValue(Attribute, NewValue);

	HandleAttributeChange(Attribute, OldValue, NewValue);
}

void UCombatAttributeSet::InitFromMetaDataTable(const UDataTable* DataTable) {
	Super::InitFromMetaDataTable(DataTable);
}

void UCombatAttributeSet::ResetHealth() {
	SetHealth(GetMaxHealth());
}

void UCombatAttributeSet::LogAttributeValue(
	const FGameplayAttribute& Attribute,
	const float Value
) const {
	const auto LogFormat = FString(TEXT("{0} attribute changed: {1}: {2}"));
	const auto LogText = FString::Format(
		*LogFormat,
		{
			*GetNameSafe(GetOwningActor()),
			*Attribute.GetName(),
			Value
		}
	);
	UE_LOG(LogTopDown2, Warning, TEXT("%s"), *LogText);
}

void UCombatAttributeSet::HandleAttributeChange(
	const FGameplayAttribute& Attribute,
	const float OldValue,
	const float NewValue
) {
	if (Attribute == GetHealthAttribute()) {
		const float NormalisedValue = NewValue < 0.f ? 0.f : NewValue;
		OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, 1.f, OldValue, NormalisedValue);
		if (NormalisedValue == 0.0f) {
			OnHealthDepleted.Broadcast();
		}
	}
}
