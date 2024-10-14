#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility_CharacterJump.h"
#include "GameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN2_API UGameplayAbility_Jump : public UGameplayAbility_CharacterJump
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* OwnerInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
