
#include "GameStateSurvival.h"

AGameStateSurvival::AGameStateSurvival() {
}

void AGameStateSurvival::BeginPlay() {
	Super::BeginPlay();
	RemainingEnemyCount = DefeatedEnemyCountToWin;
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
	return RemainingEnemyCount;
}

void AGameStateSurvival::HandleEnemyDefeated(AActor* Enemy) {
	RemainingEnemyCount--;
	if (RemainingEnemyCount <= 0) {
		OnAllEnemiesDefeated.Broadcast();
	}
}

void AGameStateSurvival::HandleAllEnemiesRemoved() {
}
