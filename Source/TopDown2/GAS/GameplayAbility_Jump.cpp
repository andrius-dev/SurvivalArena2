#include "TopDown2/GAS/GameplayAbility_Jump.h"

#include "GameFramework/Character.h"
#include "TopDown2/TopDown2.h"
#include "TopDown2/Util/Log.h"


UGameplayAbility_Jump::UGameplayAbility_Jump() {
	bReplicateEndAbility = true;
}

void UGameplayAbility_Jump::OnGiveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec
) {
	Super::OnGiveAbility(ActorInfo, Spec);
	TempOwnerInfo = ActorInfo;
	TempHandle = Spec.Handle;
	
	const auto Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->LandedDelegate.AddDynamic(
		this,
		&UGameplayAbility_Jump::OnLanded
	);
	Character->OnReachedJumpApex.AddDynamic(
		this,
		&UGameplayAbility_Jump::OnReachedJumpApex	
	);
}

void UGameplayAbility_Jump::OnRemoveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec
) {
	Super::OnRemoveAbility(ActorInfo, Spec);
	
	const auto Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->LandedDelegate.RemoveAll(this);
	Character->OnReachedJumpApex.RemoveAll(this);
}

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
	UE_LOG(LogTopDown2, All, TEXT("Activating jump"))

	this->TempHandle = Handle;
	this->TempOwnerInfo = OwnerInfo; 
	this->TempActivationInfo = ActivationInfo;
	const auto Character = CastChecked<ACharacter>(OwnerInfo->AvatarActor.Get());
	
	Character->Jump();
}

void UGameplayAbility_Jump::InputPressed(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo
) {
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

bool UGameplayAbility_Jump::CanActivateAbility(
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

void UGameplayAbility_Jump::InputReleased(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo
) {
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_Jump::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility
) {
	Super::CancelAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		bReplicateCancelAbility
	);
}

void UGameplayAbility_Jump::BeginDestroy() {
	Super::BeginDestroy();
	TempOwnerInfo = nullptr;
}

void UGameplayAbility_Jump::OnLanded(const FHitResult& HitResult) {
	if (TempOwnerInfo == nullptr) {
		UE_LOG(LogTopDown2, Error, TEXT("OwnerInfo null, can't end ability"));
		return;
	}
	EndAbility(TempHandle, TempOwnerInfo, TempActivationInfo, bReplicateEndAbility, false);
	UE_LOG(LogTopDown2, All, TEXT("Jump ability ended"));
}

void UGameplayAbility_Jump::OnReachedJumpApex() {
}
