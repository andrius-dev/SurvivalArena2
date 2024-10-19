#include "CharacterSpawner.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

ACharacterSpawner::ACharacterSpawner() {
	PrimaryActorTick.bCanEverTick = false;
	SpawnedClass = ABasicEnemyCharacter::StaticClass();
	ActorName = "DefaultName";
}

void ACharacterSpawner::BeginPlay() {
	Super::BeginPlay();
}

// todo maybe this should be implemented in game mode
AActor* ACharacterSpawner::LoadCharacterWithBehaviorTree(UBehaviorTree* BehaviorTree) {
	const auto LoadedPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
		this,
		SpawnedClass,
		BehaviorTree,
		GetActorLocation(),
		GetActorRotation(),
		bNoCollisionFail
	);
	OnCharacterLoaded.Broadcast(
		LoadedPawn,
		this
	);
	return LoadedPawn;
}

void ACharacterSpawner::SpawnCharacter(AActor* Actor) {
	Actor->SetActorLocationAndRotation(
		GetActorLocation(),
		GetActorRotation()
	);
	
}
