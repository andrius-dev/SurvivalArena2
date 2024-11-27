#include "GameStateSpawnDestruction.h"

#include "CharacterSpawner.h"
#include "Kismet/GameplayStatics.h"

void AGameStateSpawnDestruction::HandleEnemyDefeated_Implementation(UObject* Enemy) {
	IGameStateInterface::HandleEnemyDefeated_Implementation(Enemy);
}

void AGameStateSpawnDestruction::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(this, ACharacterSpawner::StaticClass(), ActiveSpawners);
}
