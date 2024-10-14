#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameplayAbility_MeleeAttack.generated.h"

class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class TOPDOWN2_API UGameplayAbility_MeleeAttack : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
public:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;
	
	virtual bool ShouldActivateAbility(ENetRole Role) const override;
	
	virtual bool CanBeCanceled() const override;
	
	virtual void OnGiveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec
	) override;

protected:
	
	virtual void SendGameplayEvent(
		FGameplayTag EventTag,
		FGameplayEventData Payload
	) override;
};
