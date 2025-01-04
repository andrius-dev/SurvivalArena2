#include "GameStateSpawnDestruction.h"

#include "CharacterSpawner.h"
#include "Kismet/GameplayStatics.h"

void AGameStateSpawnDestruction::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(
		this,
		ACharacterSpawner::StaticClass(),
		ActiveSpawners
	);
	OnSpawnerCountChanged.Broadcast(0, ActiveSpawners.Num());
}

void AGameStateSpawnDestruction::HandleEnemyDefeated_Implementation(AActor* Enemy) {
	const int OldCount = ActiveEnemies.Num();
	ActiveEnemies.Remove(Enemy);
	OnEnemyCountChanged.Broadcast(OldCount, ActiveEnemies.Num());

	const int OldScore = Score;
	Score += ScoreDestroySpawner;
	OnScoreChanged.Broadcast(OldScore, Score);

	CheckIfLevelCleared();
}

void AGameStateSpawnDestruction::HandleEnemySpawned_Implementation(AActor* Enemy) {
	const int OldCount = ActiveEnemies.Num();
	ActiveEnemies.Add(Enemy);
	OnEnemyCountChanged.Broadcast(OldCount, ActiveEnemies.Num());
}

void AGameStateSpawnDestruction::HandleSpawnerDestroyed(AActor* Spawner) {
	const int OldCount = ActiveSpawners.Num();
	ActiveSpawners.Remove(Spawner);
	OnSpawnerCountChanged.Broadcast(OldCount, ActiveSpawners.Num());

	const int OldScore = Score;
	Score += ScoreDestroySpawner;
	OnScoreChanged.Broadcast(OldScore, Score);
	
	CheckIfLevelCleared();
}

void AGameStateSpawnDestruction::SetSpawners(const TArray<AActor*>& Spawners) {
	ActiveSpawners = Spawners;
}

float AGameStateSpawnDestruction::GetScore_Implementation() {
	return Score;
}

void AGameStateSpawnDestruction::CheckIfLevelCleared() {
	if (ActiveEnemies.IsEmpty() && ActiveSpawners.IsEmpty()) {
		OnLevelCleared.Broadcast();
	}
}
