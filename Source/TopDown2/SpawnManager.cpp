#include "SpawnManager.h"

#include "AIController.h"
#include "CharacterSpawner.h"
#include "TopDown2.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ASpawnManager::ASpawnManager() {
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnManager::BeginPlay() {
	Super::BeginPlay();
	
	auto TempSpawners = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(
		this,
		ACharacterSpawner::StaticClass(),
		TempSpawners
	);
	
	for (const auto TempActor : TempSpawners) {
		SpawnersList.Add(Cast<ACharacterSpawner>(TempActor));
	}
}

TArray<AActor*> ASpawnManager::GetCharactersPool() {
	return CharactersPool;
}

TArray<AActor*> ASpawnManager::LoadEnemiesAtStartLocation(UBehaviorTree* BehaviorTree) {
	for (const auto Spawner : SpawnersList) {
		const auto NewPawn = Spawner->LoadCharacterWithBehaviorTree(BehaviorTree);
		CharactersPool.Add(NewPawn);
	}
	return CharactersPool;
}

void ASpawnManager::MoveEnemiesToSpawners(TArray<AActor*> const EnemiesToMove) {
	const int CharactersCount = EnemiesToMove.Num();
	for (int i = 0; i < SpawnersList.Num(); i++) {
		if (i >= CharactersCount) {
			break;
		}
		const auto Spawner = SpawnersList[i];
		const auto Enemy  = EnemiesToMove[i];
		if (!Enemy) {
			UE_LOG(LogTopDown2, Error, TEXT("MoveEnemiesToSpawners element is null"));
			continue;
		}
		Enemy->SetActorLocationAndRotation(
			Spawner->GetActorLocation(),
			Spawner->GetActorRotation()
		);
	}
}

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void ASpawnManager::AddCharacterToList(ACharacter* const NewCharacter) {
	CharactersPool.Add(NewCharacter);
	UE_LOG(LogTemp, Display, TEXT("Spawning Character"));
}
