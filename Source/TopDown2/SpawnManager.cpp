#include "SpawnManager.h"

#include "AIController.h"
#include "CharacterSpawner.h"
#include "TopDown2.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ASpawnManager::ASpawnManager() {
	PrimaryActorTick.bCanEverTick = false;
	SpawnMode = EEnemySpawnMode::CustomCount;
	bNoCollisionFail = false;
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

TArray<AActor*> ASpawnManager::InitializeEnemies(
	UBehaviorTree* BehaviorTree,
	EEnemySpawnMode Mode,
	TArray<FSpawnParams> SpawnParams
) {
	SpawnMode = Mode;

	switch (SpawnMode) {
	case EEnemySpawnMode::SpawnerCountMax:
		return InitializeEnemiesSpawnerCountMode(BehaviorTree);
	case EEnemySpawnMode::CustomCount:
		return InitializeEnemiesCustomCountMode(BehaviorTree, SpawnParams);
	}

	return InitializeEnemiesSpawnerCountMode(BehaviorTree);
}

TArray<AActor*> ASpawnManager::InitializeEnemiesSpawnerCountMode(
	UBehaviorTree* BehaviorTree
) {
	for (const auto Spawner : SpawnersList) {
		const auto NewPawn = Spawner->LoadCharacterWithBehaviorTree(
			BehaviorTree
		);
		CharactersPool.Add(NewPawn);
	}
	return CharactersPool;
}

TArray<AActor*> ASpawnManager::InitializeEnemiesCustomCountMode(
	UBehaviorTree* BehaviorTree,
	const TArray<FSpawnParams>& SpawnParams
) {
	for (int i = 0; i < SpawnParams.Num(); i++) {
		const auto SpawnParam = SpawnParams[i];
		for (int j = 0; j < SpawnParam.Count; j++) {
			const auto NewPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
				this,
				SpawnParam.EnemyClass,
				BehaviorTree,
				GetActorLocation(),
				GetActorRotation(),
				bNoCollisionFail
			);
			CharactersPool.Add(NewPawn);
		}
	}
	return CharactersPool;
}

TArray<AActor*> ASpawnManager::GetCharactersPool() {
	return CharactersPool;
}

void ASpawnManager::SpawnEnemy(
	ABasicEnemyCharacter* EnemyToSpawn,
	ACharacterSpawner* Spawner
) {
	EnemyToSpawn->SetActorLocationAndRotation(
		Spawner->GetActorLocation(),
		Spawner->GetActorRotation()
	);
	EnemyToSpawn->SetState(EEnemyGameState::Active);
}

void ASpawnManager::MoveEnemiesToSpawners(TArray<AActor*> const EnemiesToMove) {
	switch (SpawnMode) {
	case EEnemySpawnMode::CustomCount:
		MoveEnemiesToSpawnersCustomCountMode(EnemiesToMove);
		break;
	case EEnemySpawnMode::SpawnerCountMax:
		MoveEnemiesToSpawnerCountMode(EnemiesToMove);
		break;
	}
}

void ASpawnManager::MoveEnemiesToSpawnerCountMode(
	TArray<AActor*> const EnemiesToMove
) {
	const int CharactersCount = EnemiesToMove.Num();
	for (int i = 0; i < SpawnersList.Num(); i++) {
		if (i >= CharactersCount) {
			break;
		}
		const auto Spawner = SpawnersList[i];
		const auto Enemy = EnemiesToMove[i];
		if (!Enemy) {
			UE_LOG(
				LogTopDown2,
				Error,
				TEXT("MoveEnemiesToSpawners element is null")
			);
			continue;
		}
		Enemy->SetActorLocationAndRotation(
			Spawner->GetActorLocation(),
			Spawner->GetActorRotation()
		);
	}
}

void ASpawnManager::MoveEnemiesToSpawnersCustomCountMode(
	TArray<AActor*> const EnemiesToMove
) {
}

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void ASpawnManager::AddCharacterToList(ACharacter* const NewCharacter) {
	CharactersPool.Add(NewCharacter);
	UE_LOG(LogTemp, Display, TEXT("Spawning Character"));
}
