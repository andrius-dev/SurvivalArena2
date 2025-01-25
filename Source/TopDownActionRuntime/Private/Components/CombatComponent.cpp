#include "Components/CombatComponent.h"

#include "WeaponMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Util/Log.h"

UCombatComponent::UCombatComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	ActorsToIgnoreTrace = TArray<AActor*>();
	bDamageActorsOfSelfClass = false;
	AttackTraceHitColor = FLinearColor::Green;
	AttackTraceMissColor = FLinearColor::Red;
}

void UCombatComponent::BeginPlay() {
	Super::BeginPlay();

	InitAbilitySystem();
	
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

const TArray<AActor*> UCombatComponent::DetectMeleeHits(
	const TArray<AActor*>& ActorsToIgnore
) {
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::LineTraceMulti(
		GetOwner(),
		GetSocketLocation(BladeStart),
		GetSocketLocation(BladeEnd),
		ETraceTypeQuery::TraceTypeQuery2,
		false,	// bTraceComplex
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true,
		AttackTraceMissColor,
		AttackTraceHitColor
	);

	// todo filter out actors that don't have health
	const auto FilteredResults = HitResults.FilterByPredicate([](const FHitResult& HitResult) {
			return true;
        }
	);

	if (!FilteredResults.IsEmpty()) {
		OnDetectedMeleeHit.Broadcast(this, FilteredResults);
	}

	auto HitActors = TArray<AActor*>();
	for (const auto& HitResult : HitResults) {
		HitActors.Add(HitResult.GetActor());
	}
	
	return HitActors;
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
	// todo implement defeat started and ended.
	AttributeSet->OnHealthDepleted.AddDynamic(this, &ThisClass::HandleDefeatStarted);
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
	// UE_LOG(LogTopDown2, All, TEXT("Health changed: %hs"), NewValue);
	OnHealthChanged.Broadcast(this, OldValue, NewValue, Instigator);
}

void UCombatComponent::HandleDefeatStarted() {
	OnDefeatStarted.Broadcast(GetOwner());
}

void UCombatComponent::HandleDefeatEnded() {
	OnDefeatEnded.Broadcast(GetOwner());
}
