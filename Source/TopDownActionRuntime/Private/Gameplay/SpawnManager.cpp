#include "SpawnManager.h"

#include "AIController.h"
#include "CharacterSpawner.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Enemies/EnemyCharacterInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Util/Log.h"


ASpawnManager::ASpawnManager() {
	PrimaryActorTick.bCanEverTick = false;
	bNoCollisionFail = false;
	InactiveEnemyPool = TArray<UObject*>();
	ActiveEnemyPool = TArray<UObject*>();	
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

TArray<UObject*>
ASpawnManager::InitEnemyPool(TArray<FSpawnParams> EnemiesToInitialize) {
	InactiveEnemyPool.Empty();
	ActiveEnemyPool.Empty();

	for (int i = 0; i < EnemiesToInitialize.Num(); i++) {
		const auto SpawnParam = EnemiesToInitialize[i];
		// skip adding this enemy class if it doesn't implement the interface
		if (!SpawnParam.EnemyClass->ImplementsInterface(
			UEnemyCharacterInterface::StaticClass()
		)) {
			UE_LOG(
				LogTopDown2,
				Warning,
				TEXT("%s doesn't implement UEnemyCharacter"),
				*SpawnParam.EnemyClass.Get()->GetName()
			);
			continue;
		}

		for (int j = 0; j < SpawnParam.Count; j++) {
			// todo don't run behavior tree on spawn! this function does
			const auto SpawnedActor =
				UAIBlueprintHelperLibrary::SpawnAIFromClass(
					this,
					SpawnParam.EnemyClass,
					SpawnParam.BehaviorTree,
					GetActorLocation(),
					GetActorRotation(),
					bNoCollisionFail
				);
			if (!SpawnedActor || !SpawnedActor->Implements<UEnemyCharacterInterface>()) {
				continue;
			}
			SpawnedActor->SetActorLocationAndRotation(
				GetActorLocation(),
				GetActorRotation()
			);
			IEnemyCharacterInterface::Execute_SetState(
				SpawnedActor,
				EEnemyGameState::Inactive
			);

			InactiveEnemyPool.Add(SpawnedActor);
		}
	}

	return InactiveEnemyPool;
}

void ASpawnManager::AddEnemyToPool(FSpawnParams EnemyToAdd) {
}

TArray<UObject*>
ASpawnManager::SpawnEnemiesOnAllSpawnersFromPoolTop() {
	for (int i = 0; i < SpawnersList.Num(); i++) {
		if (i >= InactiveEnemyPool.Num()) {
			break;
		}
		const auto Spawner = SpawnersList[i];
		const auto Enemy = InactiveEnemyPool[i];
		const bool ImplementsInterface =
			Enemy->GetClass()->ImplementsInterface(
				UEnemyCharacterInterface::StaticClass()
			);
		if (ImplementsInterface) {
			SpawnEnemy(Enemy, Spawner);
		} else {
			UE_LOG(
				LogTopDown2,
				Error,
				TEXT(
					"Trying to spawn  enemy that doesn't implement IEnemyCharacter"
				)
			);
			UKismetSystemLibrary::QuitGame(
				this,
				nullptr,
				EQuitPreference::Quit,
				true
			);
		}
	}

	//todo invalid return value
	return ActiveEnemyPool;
}

bool ASpawnManager::ReturnEnemyToPool(
	UObject* Enemy
) {
	InactiveEnemyPool.Add(Enemy);
	const auto RemovedCount = ActiveEnemyPool.Remove(Enemy);
	ActiveEnemyPool.Remove(Enemy);
	
	if (RemovedCount <= 0) {
		UE_LOG(
			LogTopDown2,
			Error,
			TEXT("Failed to return defeated enemy to pool")
		);
        UKismetSystemLibrary::QuitGame(
                this,
                nullptr,
                EQuitPreference::Quit,
                true
            );
		return false;
	}

	IEnemyCharacterInterface::Execute_SetState(
		Enemy,
		EEnemyGameState::Inactive
	);
	return true;
}

TScriptInterface<IEnemyCharacterInterface>
ASpawnManager::GetRandomInactiveEnemyFromPool() {
	if (InactiveEnemyPool.IsEmpty()) {
		return nullptr;
	}
	return InactiveEnemyPool.Top();
}

TArray<UObject*> ASpawnManager::GetActiveEnemies() {
	return ActiveEnemyPool;
}

TArray<UObject*> ASpawnManager::GetInactiveEnemies() {
	return InactiveEnemyPool;
}

void ASpawnManager::SpawnEnemy(
	UObject* EnemyToSpawn,
	const ACharacterSpawner* Spawner
) {
	SpawnEnemyAtLocation(EnemyToSpawn, Spawner->GetActorLocation(), Spawner->GetActorRotation());	
}

void ASpawnManager::SpawnEnemyAtLocation(
	UObject* EnemyToSpawn,
	const FVector& Location,
	const FRotator& Rotation
) {
	if (!EnemyToSpawn) {
		UE_LOG(LogTopDown2, Error, TEXT("Enemy is null"));
		return;
	}
	const bool bImplementsInterface =
		EnemyToSpawn->GetClass()->ImplementsInterface(UEnemyCharacterInterface::StaticClass());
	if (!bImplementsInterface) {
		UE_LOG(LogTopDown2, Error, TEXT("Enemy doesnt' impelement the interface"));
		return;
	}

	ActiveEnemyPool.Add(EnemyToSpawn);
	InactiveEnemyPool.Remove(EnemyToSpawn);
	const auto EnemyActor = CastChecked<AActor>(EnemyToSpawn);

	EnemyActor->SetActorLocationAndRotation(Location, Rotation);

	IEnemyCharacterInterface::Execute_SetState(
		EnemyActor,
		EEnemyGameState::Active
	);
	OnEnemySpawned.Broadcast(EnemyToSpawn);
	
	IEnemyCharacterInterface::Execute_EventSpawned(EnemyToSpawn, nullptr);
}

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}
