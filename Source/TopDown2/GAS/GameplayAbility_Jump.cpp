#include "TopDown2/GAS/GameplayAbility_Jump.h"

#include "GameFramework/Character.h"
#include "TopDown2/TopDown2.h"

void UGameplayAbility_Jump::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
) {
	if (!CommitAbility(Handle, OwnerInfo, ActivationInfo)) {
		UE_LOG(LogTopDown2, Error, TEXT("Failed to activate ability"));
		return;
	}

	ACharacter* Character = CastChecked<ACharacter>(OwnerInfo->AvatarActor.Get());
	Character->Jump();
}
