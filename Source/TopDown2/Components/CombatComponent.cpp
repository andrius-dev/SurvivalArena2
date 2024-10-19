#include "TopDown2/Components/CombatComponent.h"

#include <set>

#include "HealthComponent.h"
#include "WeaponMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "TopDown2/TopDown2.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	HitRadius = DEFAULT_HIT_RADIUS;
	ActorsToIgnoreTrace = TArray<AActor*>();
	bDamageActorsOfSelfClass = false;
	AttackTraceHitColor = FLinearColor::Green;
	AttackTraceMissColor = FLinearColor::Red;
}

void UCombatComponent::BeginPlay()
{
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
			BladeStart = EquippedWeaponMesh->GetSocketByName("SocketBladeStart");
		}
		if (!BladeEnd) {
			BladeEnd = EquippedWeaponMesh->GetSocketByName("SocketBladeEnd");
		}
	}
}

FVector UCombatComponent::GetSocketLocation(const UStaticMeshSocket* Socket) const {
	FTransform Transform;
	bool IsSuccess = Socket->GetSocketTransform(Transform, EquippedWeaponMesh);
	if (!IsSuccess) {
		UE_LOG(LogTopDown2, Error, TEXT("Failed to get socket transform"));
		return FVector();
	}
	return Transform.GetLocation();
}

// todo: call events when another actor is hit instead of modifying it
void UCombatComponent::DetectMeleeHits() {
	TArray<FHitResult> HitResults;
	// todo bad check

	if (!BladeStart || !BladeEnd) {
		UE_LOG(LogTopDown2, Error, TEXT("Invalid socket locations"))
		return;	
	}

	UKismetSystemLibrary::LineTraceMulti(
		GetOwner(),
		GetSocketLocation(BladeStart),
		GetSocketLocation(BladeEnd),
		ETraceTypeQuery::TraceTypeQuery2,
		false, // bTraceComplex
		ActorsToIgnoreTrace,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true,
		AttackTraceMissColor,
		AttackTraceHitColor
	);
	// UKismetSystemLibrary::SphereTraceMulti(
	// 	GetOwner(),
	// 	GetSocketLocation(BladeStart),
	// 	GetSocketLocation(BladeEnd),
	// 	HitRadius,
	// 	ETraceTypeQuery::TraceTypeQuery2,
	// 	false, // bTraceComplex
	// 	ActorsToIgnoreTrace,
	// 	EDrawDebugTrace::ForDuration,
	// 	HitResults,
	// 	true // bIgnoreSelf
	// );

	auto HitActorsIdSet = std::set<uint32>();
	
	for (const FHitResult& TempResult : HitResults) {
		const auto HitActor = TempResult.GetActor();
		if (!HitActor) {
			continue;
		}
		const auto HealthComponent = HitActor->FindComponentByClass<UHealthComponent>();
		
		if (!HealthComponent ||
			HitActorsIdSet.contains(HitActor->GetUniqueID()) ||
			(!bDamageActorsOfSelfClass && HitActor->IsA(GetOwner()->StaticClass()))
		) {
			continue;
		}
		const bool bTagIgnored = TagsToIgnoreDamage.ContainsByPredicate([HitActor](const FName& Tag) {
			return HitActor->ActorHasTag(Tag);
		});
		if (bTagIgnored) {
			continue;		
		}
		
		HitActorsIdSet.insert(HitActor->GetUniqueID());
		HealthComponent->TakeDamage(DamagePerHit, this->GetOwner());
	}
}
