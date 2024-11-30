#include "GameStateSpawnDestruction.h"

#include "CharacterSpawner.h"
#include "Kismet/GameplayStatics.h"

void AGameStateSpawnDestruction::HandleEnemyDefeated_Implementation(AActor* Enemy) {
	ActiveEnemies.Remove(Enemy);
	Score += ScoreDestroySpawner;
	CheckIfLevelCleared();
}

void AGameStateSpawnDestruction::HandleEnemySpawned_Implementation(AActor* Enemy) {
	ActiveEnemies.Add(Enemy);
}

void AGameStateSpawnDestruction::HandleSpawnerDestroyed(AActor* Spawner) {
	ActiveSpawners.Remove(Spawner);
	Score += ScoreDestroySpawner;
	CheckIfLevelCleared();
}

void AGameStateSpawnDestruction::SetSpawners(TArray<AActor*> Spawners) {
	ActiveSpawners = Spawners;
}

float AGameStateSpawnDestruction::GetScore_Implementation() {
	return Score;
}

void AGameStateSpawnDestruction::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(this, ACharacterSpawner::StaticClass(), ActiveSpawners);
}

void AGameStateSpawnDestruction::CheckIfLevelCleared() {
	if (ActiveEnemies.IsEmpty() && ActiveSpawners.IsEmpty()) {
		OnLevelCleared.Broadcast();
	}	
}
