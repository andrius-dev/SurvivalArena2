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
	ActorsToIgnore = TArray<AActor*>();
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const auto Owner = MakeWeakObjectPtr(GetOwner());
	if (Owner == nullptr) {
		return;
	}

	EquippedWeaponMesh = Owner->FindComponentByClass<UWeaponMeshComponent>();
	
	if (EquippedWeaponMesh != nullptr) {
		BladeStart = EquippedWeaponMesh->GetSocketByName("bladeStart");
		BladeEnd = EquippedWeaponMesh->GetSocketByName("bladeEnd");
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

	if (BladeStart == nullptr || BladeEnd == nullptr) {
		UE_LOG(LogTopDown2, Error, TEXT("Invalid socket locations"))
		return;	
	}
	
	UKismetSystemLibrary::SphereTraceMulti(
		GetOwner(),
		GetSocketLocation(BladeStart),
		GetSocketLocation(BladeEnd),
		HitRadius,
		ETraceTypeQuery::TraceTypeQuery2,
		false, // bTraceComplex
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true // bIgnoreSelf
	);

	auto HitIdSet = std::set<uint32>();
	
	for (const FHitResult& TempResult : HitResults) {
		const auto Actor = TempResult.GetActor();
		if (!Actor) {
			continue;
		}
		const auto HealthComponent = Actor->FindComponentByClass<UHealthComponent>();
		if (!HealthComponent) {
			continue;
		}
		if (HitIdSet.contains(Actor->GetUniqueID())) {
			continue;		
		}
		
		HitIdSet.insert(Actor->GetUniqueID());
		// todo event
		HealthComponent->TakeDamage(DamagePerHit);
	}
}
