#include "CharacterSpawner.h"

#include "Enemies/BasicEnemy/BasicEnemyCharacter.h"

ACharacterSpawner::ACharacterSpawner() {
	PrimaryActorTick.bCanEverTick = false;
	SpawnedClass = ABasicEnemyCharacter::StaticClass();
	ActorName = "DefaultName";
}

void ACharacterSpawner::BeginPlay() {
	Super::BeginPlay();
}

// todo maybe this should be implemented in game mode
AActor* ACharacterSpawner::SpawnCharacter() const {
	return GetWorld()->SpawnActor(
		SpawnedClass.Get(),
		&GetActorTransform()
	);
}
