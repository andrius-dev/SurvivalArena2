#pragma once

#include <new>
#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class ACharacterSpawner;

USTRUCT(BlueprintType, Category="GameState")
struct TOPDOWN2_API FSpawnParams {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category=AI)
	TSubclassOf<ACharacter> EnemyClass;
	
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

	/**
	* @return instantiated inactive enemies 
	* */
	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<UObject*> InitEnemyPool(TArray<FSpawnParams> EnemiesToInitialize);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddEnemyToPool(FSpawnParams EnemyToAdd);
	
	/**
	* @return activated enemies
	* */
	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<UObject*> SpawnEnemiesOnAllSpawnersFromPoolTop();

	/**
	* Resets defeated enemy and marks as inactive
	* @return Whether given Enemy was found in pool and reset
	*/
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool ReturnEnemyToPool(UObject* Enemy);

	/**
	* Gives enemy for spawning it later 
	*/
	UFUNCTION(BlueprintCallable, Category="AI")
	TScriptInterface<IEnemyCharacterInterface> GetRandomInactiveEnemyFromPool();

	/**
	* Activates given enemy and moves it to Spawner if it belongs to pool 
	*/
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SpawnEnemy(UObject* EnemyToSpawn, const ACharacterSpawner* Spawner);
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<UObject*> GetActiveEnemies();
	
	UFUNCTION(BlueprintCallable, Category="AI")
	TArray<UObject*> GetInactiveEnemies();
	
	UPROPERTY()
	bool bNoCollisionFail;
	
private:
	UPROPERTY()
	TArray<UObject*> ActiveEnemyPool;
	
	UPROPERTY()
	TArray<UObject*> InactiveEnemyPool;
	
	UPROPERTY()
	TArray<ACharacterSpawner*> SpawnersList;
};
