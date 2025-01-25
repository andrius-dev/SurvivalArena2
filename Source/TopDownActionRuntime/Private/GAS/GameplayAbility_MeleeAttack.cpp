
#include "GameplayAbility_MeleeAttack.h"
#include "LyraGame/Character/LyraCharacter.h"
#include "HeroAbilityTask_OnTick.h"
#include "Kismet/KismetSystemLibrary.h"

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

const TArray<FHitResult> UGameplayAbility_MeleeAttack::TraceHits(
	const TArray<AActor*>& ActorsToIgnore,
	const FVector& WeaponStart,
	const FVector& WeaponEnd
) {
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::LineTraceMulti(
		Cast<AActor>(GetLyraCharacterFromActorInfo()),
		WeaponStart,
		WeaponEnd,
		ETraceTypeQuery::TraceTypeQuery2,
		false,	// bTraceComplex
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green
	);
	UE_LOG(LogTemp, Display, TEXT("Tracing"));
 
	// // todo filter out actors that don't have health
	// const auto FilteredResults = HitResults.FilterByPredicate([](const FHitResult& HitResult) {
	// 		return true;
 //        }
	// );
 //
	// if (!FilteredResults.IsEmpty()) {
	// 	OnDetectedMeleeHit.Broadcast(this, FilteredResults);
	// }
 //
	// auto HitActors = TArray<AActor*>();
	// for (const auto& HitResult : HitResults) {
	// 	HitActors.Add(HitResult.GetActor());
	// }
	//
	return HitResults;
}
