
#include "GameStateSurvival.h"

AGameStateSurvival::AGameStateSurvival() {
	EnemyIdSet = TSet<uint32>();
}

void AGameStateSurvival::BeginPlay() {
	Super::BeginPlay();
}

void AGameStateSurvival::AddEnemyIds(
	TArray<AActor*>& Enemies,
	const bool OnlyUnique
) {
	for (const auto Enemy : Enemies) {
		if (OnlyUnique) {
			if (EnemyIdSet.Contains(Enemy->GetUniqueID())) {
				continue;
			}	
		}
		EnemyIdSet.Add(Enemy->GetUniqueID());	
	}
}

void AGameStateSurvival::AddPlayerState(APlayerState* PlayerState) {
	Super::AddPlayerState(PlayerState);
}

void AGameStateSurvival::RemovePlayerState(APlayerState* PlayerState) {
	Super::RemovePlayerState(PlayerState);
}

void AGameStateSurvival::HandleBeginPlay() {
	Super::HandleBeginPlay();
}

void AGameStateSurvival::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

int AGameStateSurvival::GetRemainingEnemyCount() const {
	return EnemyIdSet.Num();
}

void AGameStateSurvival::HandleEnemyDefeated(AActor* Enemy) {
	EnemyIdSet.Remove(Enemy->GetUniqueID());
	if (EnemyIdSet.IsEmpty()) {
		OnAllEnemiesDefeated.Broadcast();
	}
}

void AGameStateSurvival::HandleEnemyAdded(AActor* Enemy) {
	if (EnemyIdSet.Contains(Enemy->GetUniqueID())) {
		// UE_LOG(LogTopDown2, Error, TEXT("Trying to add the same enemy twice"));
		return;	
	}
	EnemyIdSet.Add(Enemy->GetUniqueID());
}

void AGameStateSurvival::HandleEnemyListAdded(
	TArray<AActor*>& Enemies,
	const bool IsInitial
) {
	const bool CheckUnique = !IsInitial;
	if (IsInitial) {
		EnemyIdSet.Reset();
	}
	AddEnemyIds(Enemies, CheckUnique);	
}

void AGameStateSurvival::HandleAllEnemiesRemoved() {
	EnemyIdSet.Reset();
}
