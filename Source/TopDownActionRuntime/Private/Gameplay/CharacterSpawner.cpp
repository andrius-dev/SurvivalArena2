#include "CharacterSpawner.h"

ACharacterSpawner::ACharacterSpawner() {
	PrimaryActorTick.bCanEverTick = false;
	EnemySpawnFlow = ESpawnerType::Manual;
	AutoSpawnDelay = 0.0;
	AutoSpawnDelayOnStart = 2.0;
}

void ACharacterSpawner::BeginPlay() {
	Super::BeginPlay();
}
