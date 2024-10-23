#pragma once

#include <new>
#include "CoreMinimal.h"
#include "EnemySpawnFlow.h"
#include "GameModeSurvival.h"
#include "PooledEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class ACharacterSpawner;

USTRUCT(BlueprintType, Category="GameState")
struct FSpawnParams {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category=AI)
	TObjectPtr<UClass> EnemyClass;
	
	UPROPERTY(BlueprintReadWrite, Category=AI)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(BlueprintReadWrite, Category=AI)
	int Count;	
};

UCLASS()
class TOPDOWN2_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();

	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<FPooledEnemy> InitEnemyPool(TArray<FSpawnParams> EnemiesToInitialize);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddEnemyToPool(FSpawnParams EnemyToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<FPooledEnemy> SpawnEnemiesOnAllSpawnersFromPoolTop();

	/**
	* Resets defeated enemy and marks as inactive
	* @return Whether given Enemy was found in pool and reset
	*/
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool ReturnEnemyToPool(AActor* Enemy);

	/**
	* Gives enemy for spawning it later 
	*/
	UFUNCTION(BlueprintCallable, Category="AI")
	AActor* GetRandomInactiveEnemyFromPool();

	/**
	* Activates given enemy and moves it to Spawner if it belongs to pool 
	*/
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SpawnEnemy(AActor* EnemyToSpawn, const ACharacterSpawner* Spawner);
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<FPooledEnemy> GetEnemiesPool();
	
	UPROPERTY()
	bool bNoCollisionFail;
	
private:
	UPROPERTY()
	TArray<FPooledEnemy> EnemiesPool;
	
	UPROPERTY()
	TArray<ACharacterSpawner*> SpawnersList;
	
	UPROPERTY()
	TObjectPtr<AGameModeSurvival> GameModeSurvival = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	UEnemySpawnFlow* SpawnMode = nullptr;

	void MoveEnemiesToSpawner(TArray<FPooledEnemy*> const& EnemiesToMove);
};
