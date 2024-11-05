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

const TArray<TScriptInterface<IEnemyCharacter>>&
ASpawnManager::InitEnemyPool(TArray<FSpawnParams> EnemiesToInitialize) {
	InactiveEnemyPool.Empty();
	ActiveEnemyPool.Empty();

	for (int i = 0; i < EnemiesToInitialize.Num(); i++) {
		const auto SpawnParam = EnemiesToInitialize[i];
		// skip adding this enemy class if it doesn't implement the interface
		if (!SpawnParam.EnemyClass->ImplementsInterface(UEnemyCharacter::StaticClass())) {
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
			if (!SpawnedActor || !SpawnedActor->Implements<UEnemyCharacter>()) {
				continue;
			}
			SpawnedActor->SetActorLocationAndRotation(
				GetActorLocation(),
				GetActorRotation()
			);

			const auto SpawnedCharacter = CastChecked<ACharacter>(SpawnedActor);
            const auto AIController = Cast<AAIController>(SpawnedCharacter->GetController());

			if (AIController != NULL) {
				AIController->RunBehaviorTree(SpawnParam.BehaviorTree);
			} else {
				UE_LOG(LogTopDown2, Error, TEXT("Bleeeet"));
			}
			
			InactiveEnemyPool.Add(TScriptInterface<IEnemyCharacter>(SpawnedActor));
		}
	}

	return InactiveEnemyPool;
}

void ASpawnManager::AddEnemyToPool(FSpawnParams EnemyToAdd) {
}

TArray<TScriptInterface<IEnemyCharacter>>
ASpawnManager::SpawnEnemiesOnAllSpawnersFromPoolTop() {
	TMap<uint32, TScriptInterface<IEnemyCharacter>> EnemiesToActivate;
	for (int i = 0; i < SpawnersList.Num(); i++) {
		if (i >= InactiveEnemyPool.Num()) {
			break;
		}
		const auto Spawner = SpawnersList[i];
		const auto Enemy = InactiveEnemyPool[i];
		EnemiesToActivate.Add(
			IEnemyCharacter::Execute_GetCharacter(Enemy.GetObject())->
			GetUniqueID(),
			Enemy
		);
		SpawnEnemy(TScriptInterface(Enemy), Spawner);
	}
	InactiveEnemyPool.RemoveAll(
		[EnemiesToActivate](TScriptInterface<IEnemyCharacter> Enemy) {
			const auto Key = IEnemyCharacter
				::Execute_GetCharacter(Enemy.GetObject())->GetUniqueID();
			return EnemiesToActivate.Contains(Key);
		}
	);
	TArray<TScriptInterface<IEnemyCharacter>> SpawnedEnemies;
	EnemiesToActivate.GenerateValueArray(SpawnedEnemies);
	for(int i = 0; i < SpawnedEnemies.Num(); i++) {
		ActiveEnemyPool.Add(SpawnedEnemies[i]);
	}

	return SpawnedEnemies;
}

bool ASpawnManager::ReturnEnemyToPool(
	const TScriptInterface<IEnemyCharacter> Enemy
) {
	const auto EnemyObj = Enemy.GetObject();
	const auto MatchingEnemy =
		ActiveEnemyPool.FindByPredicate(
			[EnemyObj](const TScriptInterface<IEnemyCharacter>& PoolEnemy) {
				return PoolEnemy == EnemyObj;
			}
		);
	if (!MatchingEnemy) {
		return false;
	}
    ActiveEnemyPool.Remove(MatchingEnemy->GetObjectRef());
	InactiveEnemyPool.Add(MatchingEnemy->GetObjectRef());

	IEnemyCharacter::Execute_SetState(
		MatchingEnemy->GetObject(),
		EEnemyGameState::Inactive
	);
	// todo move to start location
	return true;
}

TScriptInterface<IEnemyCharacter>
ASpawnManager::GetRandomInactiveEnemyFromPool() {
	if (InactiveEnemyPool.IsEmpty()) {
		return nullptr;
	}
	return InactiveEnemyPool.Top();
}

TArray<TScriptInterface<IEnemyCharacter>> ASpawnManager::GetActiveEnemies() {
	return ActiveEnemyPool;
}

TArray<TScriptInterface<IEnemyCharacter>> ASpawnManager::GetInactiveEnemies() {
	return InactiveEnemyPool;
}

void ASpawnManager::SpawnEnemy(
	TScriptInterface<IEnemyCharacter> EnemyToSpawn,
	const ACharacterSpawner* Spawner
) {
	if (!EnemyToSpawn) {
		UKismetSystemLibrary::QuitGame(
			this, 
			nullptr,
			EQuitPreference::Quit,
			true // bIgnorePlatformRestrictions
		);
		return;
	}
	IEnemyCharacter::Execute_GetCharacter(EnemyToSpawn.GetObject())->
		SetActorLocationAndRotation(
			Spawner->GetActorLocation(),
			Spawner->GetActorRotation()
		);

	IEnemyCharacter::Execute_SetState(
		EnemyToSpawn.GetObject(),
		EEnemyGameState::Active
	);
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
