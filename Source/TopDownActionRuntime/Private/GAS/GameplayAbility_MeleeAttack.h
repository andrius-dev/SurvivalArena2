#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "GameplayAbility_MeleeAttack.generated.h"

class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class  UGameplayAbility_MeleeAttack : public ULyraGameplayAbility
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
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags
	) const override;
	
	virtual bool ShouldActivateAbility(ENetRole Role) const override;
	
	virtual bool CanBeCanceled() const override;
	
	virtual void OnGiveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec
	) override;

	UFUNCTION(BlueprintCallable, Category = "Melee")
	const TArray<FHitResult> TraceHits(
		const TArray<AActor*>& ActorsToIgnore,
		const FVector& WeaponStart,
		const FVector& WeaponEnd
	);

protected:
	
	virtual void SendGameplayEvent(
		FGameplayTag EventTag,
		FGameplayEventData Payload
	) override;

private:
};
