#include "SpawnManager.h"

#include "AIController.h"
#include "CharacterSpawner.h"
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
			)
			continue;
		}

		for (int j = 0; j < SpawnParam.Count; j++) {
			const auto SpawnedActor =
				UAIBlueprintHelperLibrary::SpawnAIFromClass(
					this,
					SpawnParam.EnemyClass,
					SpawnParam.BehaviorTree,
					GetActorLocation(),
					GetActorRotation(),
					bNoCollisionFail
				);
			if (!SpawnedActor || !SpawnedActor->Implements<
				UEnemyCharacterInterface>()) {
				continue;
			}
			SpawnedActor->SetActorLocationAndRotation(
				GetActorLocation(),
				GetActorRotation()
			);

			const auto SpawnedCharacter = CastChecked<ACharacter>(SpawnedActor);
			const auto AIController = Cast<AAIController>(
				SpawnedCharacter->GetController()
			);

			if (AIController) {
				AIController->RunBehaviorTree(SpawnParam.BehaviorTree);
			} else {
				UE_LOG(LogTopDown2, Error, TEXT("AI controller not found"));
			}

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
	const bool bImplementsInterface =
		EnemyToSpawn->GetClass()->ImplementsInterface(UEnemyCharacterInterface::StaticClass());
	if (!EnemyToSpawn || !bImplementsInterface) {
		UKismetSystemLibrary::QuitGame(
			this,
			nullptr,
			EQuitPreference::Quit,
			true // bIgnorePlatformRestrictions
		);
		return;
	}
	
	ActiveEnemyPool.Add(EnemyToSpawn);
	InactiveEnemyPool.Remove(EnemyToSpawn);
	const auto EnemyActor = CastChecked<AActor>(EnemyToSpawn);

	EnemyActor->SetActorLocationAndRotation(
		Spawner->GetActorLocation(),
		Spawner->GetActorRotation()
	);

	IEnemyCharacterInterface::Execute_SetState(
		EnemyToSpawn,
		EEnemyGameState::Active
	);
}

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}
