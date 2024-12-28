#include "TopDown2/Components/CombatComponent.h"

#include "WeaponMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "TopDown2/Util/Log.h"

UCombatComponent::UCombatComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	ActorsToIgnoreTrace = TArray<AActor*>();
	bDamageActorsOfSelfClass = false;
	AttackTraceHitColor = FLinearColor::Green;
	AttackTraceMissColor = FLinearColor::Red;
}

void UCombatComponent::BeginPlay() {
	Super::BeginPlay();

	const auto Owner = MakeWeakObjectPtr(GetOwner());
	if (Owner == nullptr) {
		return;
	}

	// todo init these in base characters.
	if (!EquippedWeaponMesh) {
		EquippedWeaponMesh = Owner->FindComponentByClass<UWeaponMeshComponent>();
	}
	if (EquippedWeaponMesh) {
		if (!IsValid(BladeStart)) {
			BladeStart = EquippedWeaponMesh->GetSocketByName("SocketBladeStart");
		}
		if (!IsValid(BladeEnd)) {
			BladeEnd = EquippedWeaponMesh->GetSocketByName("SocketBladeEnd");
		}
	}
	
	InitAbilitySystem();
}

FVector UCombatComponent::GetSocketLocation(const UStaticMeshSocket* Socket) const {
	FTransform Transform;
	const bool IsSuccess = Socket->GetSocketTransform(Transform, EquippedWeaponMesh);
	if (!IsSuccess) {
		UE_LOG(LogTopDown2, Error, TEXT("Failed to get socket transform"));
		return FVector();
	}
	return Transform.GetLocation();
}

const TArray<FHitResult> UCombatComponent::DetectMeleeHits() {
	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::LineTraceMulti(
		GetOwner(),
		GetSocketLocation(BladeStart),
		GetSocketLocation(BladeEnd),
		ETraceTypeQuery::TraceTypeQuery2,
		false,
		// bTraceComplex
		ActorsToIgnoreTrace,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true,
		AttackTraceMissColor,
		AttackTraceHitColor
	);

	const auto FilteredResults = HitResults.FilterByPredicate([](const FHitResult& HitResult) {
			return true;
        }
	);

	// auto HitActorsIdSet = TSet<uint32>();
	//
	// for (const FHitResult& TempResult : HitResults) {
	// 	const auto HitActor = TempResult.GetActor();
	// 	if (!HitActor) {
	// 		continue;
	// 	}
	// 	const auto CombatComponent = HitActor->FindComponentByClass<UCombatComponent>();
	// 	const bool IsAllowedToDamage =!bDamageActorsOfSelfClass && HitActor->IsA(GetOwner()->GetClass()); 
	//
	// 	if (!IsValid(CombatComponent) ||
	// 		HitActorsIdSet.Contains(HitActor->GetUniqueID()) ||
	// 		IsAllowedToDamage	
	// 	) {
	// 		continue;
	// 	}
	// 	const bool bTagIgnored = TagsToIgnoreDamage.ContainsByPredicate(
	// 		[HitActor](const FName& Tag) {
	// 			return HitActor->ActorHasTag(Tag);
	// 		}
	// 	);
	// 	if (bTagIgnored) {
	// 		continue;
	// 	}
	//
	// 	HitActorsIdSet.Add(HitActor->GetUniqueID());
	// }
	return FilteredResults;
}

void UCombatComponent::InitAbilitySystem() {
	AbilitySystemComponent = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(AbilitySystemComponent)) {
		UE_LOG(LogTopDown2, Error, TEXT("%s Failed to get ability system component"), *GetNameSafe(this));
		return;
	}
	AttributeSet = AbilitySystemComponent->GetSet<UCombatAttributeSet>();
	if (!IsValid(AttributeSet)) {
		UE_LOG(LogTopDown2, Error, TEXT("%s Has invalid attribute set"), *GetNameSafe(GetOwner()));
		return;
	}

	AttributeSet->OnHealthChanged.AddUObject(this, &UCombatComponent::HandleHealthChanged);
}

float UCombatComponent::GetMaxHealth() const {
	return IsValid(AttributeSet)
		? AttributeSet->GetMaxHealth()
		: 0.0f;
}

float UCombatComponent::GetCurrentHealth() const {
	return IsValid(AttributeSet)
		? AttributeSet->GetHealth()
		: 0.0f;
}

void UCombatComponent::SetCanReceiveDamage(bool NewDamage) {
	bCanReceiveDamage = NewDamage;
}

bool UCombatComponent::GetCanReceiveDamage() const {
	return bCanReceiveDamage;
}

void UCombatComponent::HandleHealthChanged(
	AActor* Instigator,
	AActor* DamageCauser,
	const FGameplayEffectSpec* DamageEffectSpec,
	float DamageMagnitude,
	const float OldValue,
	const float NewValue
) {
	UE_LOG(LogTopDown2, All, TEXT("Health changed: %s"), NewValue);
	OnHealthChanged.Broadcast(this, OldValue, NewValue, Instigator);
}
