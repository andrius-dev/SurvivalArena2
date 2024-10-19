#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility_CharacterJump.h"
#include "GameplayAbility_Jump.generated.h"

/**
 * 
 */
// todo: add states class/enum

enum EAbilityState {
	
};

UCLASS()
class TOPDOWN2_API UGameplayAbility_Jump : public UGameplayAbility_CharacterJump
{
	GENERATED_BODY()

public:
	UGameplayAbility_Jump();
	
	virtual void OnGiveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec
	) override;
	
	virtual void OnRemoveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec
	) override;
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* OwnerInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
	virtual void BeginDestroy() override;

protected:
	
public:
	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) override;
	
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;
	
	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) override;
	
	virtual void CancelAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility
	) override;

private:
	
	UFUNCTION()
	void OnLanded(const FHitResult& HitResult);
	
	UFUNCTION()
	void OnReachedJumpApex();
	const FGameplayAbilityActorInfo* TempOwnerInfo = nullptr;
	FGameplayAbilitySpecHandle TempHandle;
	FGameplayAbilityActivationInfo TempActivationInfo;
	
	bool bReplicateEndAbility;
};
