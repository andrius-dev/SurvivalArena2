#include "TopDown2/Components/CombatComponent.h"

#include <set>

#include "HealthComponent.h"
#include "WeaponMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MaterialEditor/MaterialEditorMeshComponent.h"
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
	auto MeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
	EquippedWeaponMesh->SetStaticMesh(MeshComponent->GetStaticMesh());
	
	if (EquippedWeaponMesh != nullptr) {
		BladeStart = EquippedWeaponMesh->GetSocketLocation("bladeStart");
		BladeEnd = EquippedWeaponMesh->GetSocketLocation("bladeEnd");
	} else {
		BladeStart = FVector();
		BladeEnd = FVector();
	}
}

void UCombatComponent::DetectMeleeHits() {
	TArray<FHitResult> HitResults;
	// todo bad check
	
    EquippedWeaponMesh = GetOwner()->FindComponentByClass<UWeaponMeshComponent>();
    const auto BladeStart1 = EquippedWeaponMesh->GetSocketLocation("bladeStart");
    const auto BladeEnd1 = EquippedWeaponMesh->GetSocketLocation("bladeEnd");
	if (BladeStart1.IsZero() || BladeEnd1.IsZero()) {
		UE_LOG(LogTopDown2, Error, TEXT("Invalid socket locations"))
		return;	
	}
	
	UKismetSystemLibrary::SphereTraceMulti(
		GetOwner(),
		BladeStart1,
		BladeEnd1,
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
		const auto HealthComponent = Actor->FindComponentByClass<UHealthComponent>();
		if (!HealthComponent) {
			continue;
		}
		if (HitIdSet.contains(Actor->GetUniqueID())) {
			continue;		
		}
		
		HitIdSet.insert(Actor->GetUniqueID());
		HealthComponent->TakeDamage(DamagePerHit);
	}
}
