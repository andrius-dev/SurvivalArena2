
#include "GameStateSurvival.h"

AGameStateSurvival::AGameStateSurvival() {
	Score = 0;
}

void AGameStateSurvival::BeginPlay() {
	Super::BeginPlay();
	RemainingEnemyCount = DefeatedEnemyCountToWin;
}

void AGameStateSurvival::AddPlayerState(APlayerState* PlayerState) {
	Super::AddPlayerState(PlayerState);
}

float AGameStateSurvival::GetScore_Implementation() {
	return Score;
}

FString AGameStateSurvival::GetFormattedScore() const {
	return FString::FromInt(Score);
}

void AGameStateSurvival::HandleEnemyDefeated_Implementation(AActor* Enemy) {
	RemainingEnemyCount--;
	// todo add score handling based on enemy
	Score += 100;
	
	if (RemainingEnemyCount <= 0) {
		OnAllEnemiesDefeated.Broadcast();
	}
}

void AGameStateSurvival::HandleEnemySpawned_Implementation(AActor* Enemy) {
	IGameStateInterface::HandleEnemySpawned_Implementation(Enemy);
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
