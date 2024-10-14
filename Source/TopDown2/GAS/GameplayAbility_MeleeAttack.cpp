
#include "GameplayAbility_MeleeAttack.h"

#include "TopDown2/TopDown2.h"

void UGameplayAbility_MeleeAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UGameplayAbility_MeleeAttack::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags
) const {
	return Super::CanActivateAbility(
		Handle,
		ActorInfo,
		SourceTags,
		TargetTags,
		OptionalRelevantTags
	);
}

bool UGameplayAbility_MeleeAttack::ShouldActivateAbility(ENetRole Role) const {
	return Super::ShouldActivateAbility(Role);
}

bool UGameplayAbility_MeleeAttack::CanBeCanceled() const {
	return Super::CanBeCanceled();
}

void UGameplayAbility_MeleeAttack::OnGiveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec
) {
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGameplayAbility_MeleeAttack::SendGameplayEvent(
	FGameplayTag EventTag,
	FGameplayEventData Payload
) {
	Super::SendGameplayEvent(EventTag, Payload);
}

