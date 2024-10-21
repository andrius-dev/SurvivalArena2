#include "SpawnManager.h"

#include "AIController.h"
#include "AudioDeviceNotificationSubsystem.h"
#include "CharacterSpawner.h"
#include "Algo/Count.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TopDown2/Util/Log.h"


ASpawnManager::ASpawnManager() {
	PrimaryActorTick.bCanEverTick = false;
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
	UEnemySpawnMode* Mode,
	// todo maybe remove 
	TArray<FSpawnParams> SpawnParams
) {
	check(Mode);
	SpawnMode = Mode;

	for (int i = 0; i < SpawnParams.Num(); i++) {
		const auto SpawnParam = SpawnParams[i];
		for (int j = 0; j < SpawnParam.Count; j++) {
			const auto NewPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
				this,
				SpawnParam.EnemyClass,
				SpawnParam.BehaviorTree,
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

void ASpawnManager::InitialEnemySpawn(int Count) {
	if (Count > CharactersPool.Num()) {
		UE_LOG(LogTopDown2, Warning, TEXT("Enemy pool is less than Count, to be implemented"));
	}

	// switch (SpawnMode->GetModeType()) {
	// case UEnemySpawnModeContinuous::StaticClass():
	// 	if (Count <= SpawnersList.Num()) {
	// 		for (int i = 0; i < Count; i++) {
	// 			const auto Spawner = SpawnersList[i];
	// 			CharactersPool[i]->SetActorLocationAndRotation(
	// 				Spawner->GetActorLocation(),
	// 				Spawner->GetActorRotation()
	// 			);
	// 		}
	// 	}
	// 	break;
	// case UEnemySpawnModeWaves::StaticClass():
	// 	UE_LOG(LogTopDown2, Warning, TEXT("Wave mode is not implemented"));
	// 	break;
	// default:
	// 	UE_LOG(LogTopDown2, Error, TEXT("Invalid SpawnMode"));
	// }
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

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void ASpawnManager::AddCharacterToList(ACharacter* const NewCharacter) {
	CharactersPool.Add(NewCharacter);
	UE_LOG(LogTemp, Display, TEXT("Spawning Character"));
}
