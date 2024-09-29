
#include "CharacterSpawner.h"

#include "Enemies/BasicEnemy/BasicEnemyCharacter.h"

// Sets default values
ACharacterSpawner::ACharacterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnedClass = ABasicEnemyCharacter::StaticClass();
	ActorName = "DefaultName";
}

// Called when the game starts or when spawned
void ACharacterSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// todo maybe this should be implemented in game mode
AActor* ACharacterSpawner::SpawnCharacter() const {
	
	return GetWorld()->SpawnActor(
		SpawnedClass.Get(),
		&GetActorTransform()
	);
}

