#pragma once

#include <new>
#include "CoreMinimal.h"
#include "GameModeSurvival.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class ACharacterSpawner;

UENUM(BlueprintType)
enum class EEnemySpawnMode : uint8 {
	SpawnerCountMax,
	CustomCount	
};

USTRUCT(BlueprintType)
struct FSpawnParams {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category=AI)
	TObjectPtr<UClass> EnemyClass;
	
	UPROPERTY(BlueprintReadOnly, Category=AI)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(BlueprintReadOnly, Category=AI)
	int Count;	
};

UCLASS()
class TOPDOWN2_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();

	UFUNCTION(BlueprintCallable, Category = "AI")
	
	TArray<AActor*> InitializeEnemies(
		EEnemySpawnMode Mode,
		TArray<FSpawnParams> SpawnParams
	);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddCharacterToList(ACharacter* NewCharacter);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MoveEnemiesToSpawners(TArray<AActor*> const EnemiesToMove);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<AActor*> GetCharactersPool();
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SpawnEnemy(ABasicEnemyCharacter* EnemyToSpawn, ACharacterSpawner* Spawner);
	
	UPROPERTY()
	bool bNoCollisionFail;
	
private:
	UPROPERTY()
	TArray<AActor*> CharactersPool;
	
	UPROPERTY()
	TArray<ACharacterSpawner*> SpawnersList;
	
	UPROPERTY()
	TObjectPtr<AGameModeSurvival> GameModeSurvival = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	EEnemySpawnMode SpawnMode;

	TArray<AActor*> InitializeEnemiesSpawnerCountMode(UBehaviorTree* BehaviorTree);
	TArray<AActor*> InitializeEnemiesCustomCountMode(const TArray<FSpawnParams>& SpawnParams);
	void MoveEnemiesToSpawnerCountMode(TArray<AActor*> const EnemiesToMove);
	void MoveEnemiesToSpawnersCustomCountMode(TArray<AActor*> const EnemiesToMove);
};
