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

TArray<FPooledEnemy> ASpawnManager::InitEnemyPool(
	TArray<FSpawnParams> EnemiesToInitialize
) {
	for (int i = 0; i < EnemiesToInitialize.Num(); i++) {
		const auto SpawnParam = EnemiesToInitialize[i];
		for (int j = 0; j < SpawnParam.Count; j++) {
			const auto NewPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
				this,
				SpawnParam.EnemyClass,
				SpawnParam.BehaviorTree,
				GetActorLocation(),
				GetActorRotation(),
				bNoCollisionFail
			);
			const auto PooledEnemy = FPooledEnemy(NewPawn, false);
			EnemiesPool.Add(PooledEnemy);
		}
	}

	return EnemiesPool;
}

void ASpawnManager::AddEnemyToPool(FSpawnParams EnemyToAdd) {
}

TArray<FPooledEnemy> ASpawnManager::SpawnEnemiesOnAllSpawnersFromPoolTop() {
	const auto InactiveEnemies =
		EnemiesPool.FilterByPredicate(
			[&](const FPooledEnemy& Enemy) {
				return !Enemy.bActive;
			}
		);

	TArray<FPooledEnemy> SpawnEnemies;
	for (int i = 0; i < SpawnersList.Num(); i++) {
		if (i >= InactiveEnemies.Num()) {
			break;
		}
		const auto Spawner = SpawnersList[i];
		const auto Enemy = EnemiesPool[i];
		SpawnEnemy(Enemy.Character, Spawner);
		SpawnEnemies.Add(Enemy);
	}
	return SpawnEnemies;
}

bool ASpawnManager::ReturnEnemyToPool(AActor* Enemy) {
	const auto MatchingEnemy =
		EnemiesPool.FindByPredicate(
			[Enemy](const FPooledEnemy& PoolEnemy) {
				return PoolEnemy.Character->GetUniqueID() == Enemy->
					GetUniqueID();
			}
		);
	if (!MatchingEnemy) {
		return false;
	}
	MatchingEnemy->bActive = false;
	const auto Character = CastChecked<ABasicEnemyCharacter>(
		MatchingEnemy->Character
	);
	Character->SetState(EEnemyGameState::Inactive);
	// todo move to start location
	return true;
}

AActor* ASpawnManager::GetRandomInactiveEnemyFromPool() {
	const auto PoolEnemy = EnemiesPool.FindByPredicate(
		[](const FPooledEnemy& TempPoolEnemy) {
			return !TempPoolEnemy.bActive;
		}
	);
	if (!PoolEnemy)	{
		return nullptr;
	}
	return PoolEnemy->Character;
}

TArray<FPooledEnemy> ASpawnManager::GetEnemiesPool() {
	return EnemiesPool;
}

void ASpawnManager::SpawnEnemy(
	AActor* EnemyToSpawn,
	const ACharacterSpawner* Spawner
) {
	EnemyToSpawn->SetActorLocationAndRotation(
		Spawner->GetActorLocation(),
		Spawner->GetActorRotation()
	);

	// todo make the fucking interface already
	if (EnemyToSpawn->IsA(ABasicEnemyCharacter::StaticClass())) {
		const auto BasicEnemy = Cast<ABasicEnemyCharacter>(EnemyToSpawn);
		BasicEnemy->SetState(EEnemyGameState::Active);
	}
}

// void ASpawnManager::SpawnEnemiesFromPool(int Count) {
// 	if (Count > EnemiesPool.Num()) {
// 		UE_LOG(
// 			LogTopDown2,
// 			Warning,
// 			TEXT("Enemy pool is less than Count, to be implemented")
// 		);
// 	}
//
// 	switch (SpawnMode->GetModeType()) {
// 	case UEnemySpawnFlowContinuous::StaticClass():
// 		if (Count <= SpawnersList.Num()) {
// 			for (int i = 0; i < Count; i++) {
// 				const auto Spawner = SpawnersList[i];
// 				EnemiesPool[i]->Character->SetActorLocationAndRotation(
// 					Spawner->GetActorLocation(),
// 					Spawner->GetActorRotation()
// 				);
// 			}
// 		}
// 		break;
// 	case UEnemySpawnModeWaves::StaticClass():
// 		UE_LOG(LogTopDown2, Warning, TEXT("Wave mode is not implemented"));
// 		break;
// 	default:
// 		UE_LOG(LogTopDown2, Error, TEXT("Invalid SpawnMode"));
// 	}
// }

void ASpawnManager::MoveEnemiesToSpawner(
	TArray<FPooledEnemy*> const& EnemiesToMove
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
		SpawnEnemy(Enemy->Character, Spawner);
	}
}

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}
