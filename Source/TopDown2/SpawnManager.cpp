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

void ASpawnManager::LoadEnemiesAtStartLocation(UBehaviorTree* BehaviorTree) {
	for (const auto Spawner : SpawnersList) {
		const auto NewActor = UAIBlueprintHelperLibrary::SpawnAIFromClass(
			this,
			Spawner->GetSpawnedClass(),
			BehaviorTree,
			GetActorLocation(),
			FRotator::ZeroRotator,
			true // bNoCollisionFail
		);
		UE_LOG(LogTopDown2, Warning, TEXT("Spawning character: %s"), *NewActor->GetActorLocation().ToString());
		const auto NewCharacter = Cast<ACharacter>(NewActor);
		CharactersPool.Add(NewCharacter);
	}
}

void ASpawnManager::MoveEnemiesToSpawners() {
	const int CharactersCount = CharactersPool.Num();
	for (int i = 0; i < SpawnersList.Num(); i++) {
		if (i >= CharactersCount) {
			break;
		}
		const auto Spawner = SpawnersList[i];
		CharactersPool[i]->SetActorLocation(Spawner->GetActorLocation());
		
		UE_LOG(LogTopDown2, Warning, TEXT("Moving character to: %s"), *Spawner->GetActorLocation().ToString());
	}
}

void ASpawnManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void ASpawnManager::AddCharacterToList(ACharacter* const NewCharacter) {
	CharactersPool.Add(NewCharacter);
	UE_LOG(LogTemp, Display, TEXT("Spawning Character"));
}
