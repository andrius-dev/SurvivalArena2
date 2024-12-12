#include "TopDown2/Components/CombatComponent.h"

#include <set>

#include "WeaponMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "TopDown2/Util/Log.h"

UCombatComponent::UCombatComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	HitRadius = DEFAULT_HIT_RADIUS;
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

	if (!EquippedWeaponMesh) {
		EquippedWeaponMesh = Owner->FindComponentByClass<UWeaponMeshComponent>();
	}
	if (EquippedWeaponMesh) {
		if (!BladeStart) {
			BladeStart = EquippedWeaponMesh->
				GetSocketByName("SocketBladeStart");
		}
		if (!BladeEnd) {
			BladeEnd = EquippedWeaponMesh->GetSocketByName("SocketBladeEnd");
		}
	}
}

FVector UCombatComponent::GetSocketLocation(
	const UStaticMeshSocket* Socket
) const {
	FTransform Transform;
	bool IsSuccess = Socket->GetSocketTransform(Transform, EquippedWeaponMesh);
	if (!IsSuccess) {
		UE_LOG(LogTopDown2, Error, TEXT("Failed to get socket transform"));
		return FVector();
	}
	return Transform.GetLocation();
}

void UCombatComponent::DetectMeleeHits() {
	TArray<FHitResult> HitResults;

	if (!BladeStart || !BladeEnd) {
		UE_LOG(LogTopDown2, Error, TEXT("Invalid socket locations"))
		return;
	}

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

	auto HitActorsIdSet = std::set<uint32>();

	for (const FHitResult& TempResult : HitResults) {
		const auto HitActor = TempResult.GetActor();
		if (!HitActor) {
			continue;
		}
		const auto CombatComponent = HitActor->FindComponentByClass<UCombatComponent>();

		if (!CombatComponent || HitActorsIdSet.contains(HitActor->GetUniqueID()) ||
			(!bDamageActorsOfSelfClass && HitActor->IsA(GetOwner()->GetClass()))
		) {
			continue;
		}
		const bool bTagIgnored = TagsToIgnoreDamage.ContainsByPredicate(
			[HitActor](const FName& Tag) {
				return HitActor->ActorHasTag(Tag);
			}
		);
		if (bTagIgnored) {
			continue;
		}

		HitActorsIdSet.insert(HitActor->GetUniqueID());
		CombatComponent->TakeDamage(DamagePerHit, this->GetOwner());
	}
}

float UCombatComponent::GetMaxHealth() const {
	return MaxHealth;
}

void UCombatComponent::SetCurrentHealth(const float Health, const bool bNotify) {
	if (bNotify) {
		OnHealthChanged.Broadcast(
			this,
			CurrentHealth,
			Health,
			this->GetOwner()
		);
	}
	CurrentHealth = Health;
}

void UCombatComponent::ResetHealth() {
	CurrentHealth = MaxHealth;
}

float UCombatComponent::GetCurrentHealth() const {
	return CurrentHealth;
}

void UCombatComponent::SetCanReceiveDamage(bool NewDamage) {
	bCanReceiveDamage = NewDamage;
}

bool UCombatComponent::GetCanReceiveDamage() const {
	return bCanReceiveDamage;
}

float UCombatComponent::TakeDamage(const float Amount, AActor* Initiator) {
	if(!bCanReceiveDamage) {
		return 0.0f;
	}
	const float OldHealth = CurrentHealth;
	CurrentHealth -= Amount;
	if (CurrentHealth < 0.f) {
		CurrentHealth = 0.f;
	}

	OnHealthChanged.Broadcast(
		this,
		OldHealth,
		CurrentHealth,
		Initiator
	);

	if (CurrentHealth == 0) {
		OnDefeat.Broadcast(GetOwner());
	}

	// there will be calculations later
	return Amount;
}
