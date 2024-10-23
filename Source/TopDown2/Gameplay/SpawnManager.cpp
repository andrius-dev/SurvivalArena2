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

TArray<UEnemyCharacter*> ASpawnManager::InitEnemyPool(
	TArray<FSpawnParams> EnemiesToInitialize
) {
	InactiveEnemyPool.Reset();
	ActiveEnemyPool.Reset();

	for (int i = 0; i < EnemiesToInitialize.Num(); i++) {
		const auto SpawnParam = EnemiesToInitialize[i];
		for (int j = 0; j < SpawnParam.Count; j++) {
			const auto NewPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
				this,
				SpawnParam.EnemyClass.GetClass(),
				SpawnParam.BehaviorTree,
				GetActorLocation(),
				GetActorRotation(),
				bNoCollisionFail
			);
			InactiveEnemyPool.Add(Cast<UEnemyCharacter>(NewPawn));
		}
	}

	return InactiveEnemyPool;
}

void ASpawnManager::AddEnemyToPool(FSpawnParams EnemyToAdd) {
}

TArray<const UEnemyCharacter*> ASpawnManager::SpawnEnemiesOnAllSpawnersFromPoolTop() {
	TMap<uint32, const UEnemyCharacter*> EnemiesToActivate;
	for (int i = 0; i < SpawnersList.Num(); i++) {
		if (i >= InactiveEnemyPool.Num()) {
			break;
		}
		const auto Spawner = SpawnersList[i];
		const auto Enemy = InactiveEnemyPool[i];
		EnemiesToActivate.Add(Enemy->GetUniqueID(), Enemy);
		SpawnEnemy(Enemy, Spawner);
	}
	InactiveEnemyPool.RemoveAll(
		[EnemiesToActivate](const UEnemyCharacter* Enemy) {
			return EnemiesToActivate.Contains(Enemy->GetUniqueID());
		}
	);
	TArray<const UEnemyCharacter*> SpawnedEnemies;
	EnemiesToActivate.GenerateValueArray(SpawnedEnemies);

	return SpawnedEnemies;
}

bool ASpawnManager::ReturnEnemyToPool(AActor* Enemy) {
	const auto MatchingEnemy =
		ActiveEnemyPool.FindByPredicate(
			[Enemy](const FPooledEnemy& PoolEnemy) {
				return Cast<AActor>(PoolEnemy.Character) == Enemy;
			}
		);
	if (!MatchingEnemy) {
		return false;
	}
	(*MatchingEnemy)->SetState(EEnemyGameState::Inactive);
	// todo move to start location
	return true;
}

AActor* ASpawnManager::GetRandomInactiveEnemyFromPool() {
	const auto PoolEnemy = ActiveEnemyPool.FindByPredicate(
		[](const FPooledEnemy& TempPoolEnemy) {
			return !TempPoolEnemy.bActive;
		}
	);
	if (!PoolEnemy) {
		return nullptr;
	}
	return IEnemyCharacter::Execute_GetCharacter(*PoolEnemy);
}

TArray<UEnemyCharacter*> ASpawnManager::GetActiveEnemies() {
	return ActiveEnemyPool;
}

TArray<UEnemyCharacter*> ASpawnManager::GetInactiveEnemies() {
	return InactiveEnemyPool;
}

void ASpawnManager::SpawnEnemy(
	UEnemyCharacter* EnemyToSpawn,
	const ACharacterSpawner* Spawner
) {
	if (!EnemyToSpawn) {
		return;
	}
	EnemyToSpawn->GetCharacter()->SetActorLocationAndRotation(
		Spawner->GetActorLocation(),
		Spawner->GetActorRotation()
	);

	EnemyToSpawn->SetState(EEnemyGameState::Active);
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

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}
