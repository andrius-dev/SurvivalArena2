#pragma once

#include <new>
#include "CoreMinimal.h"
#include "EnemySpawnMode.h"
#include "GameModeSurvival.h"
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
	TArray<AActor*> InitializeEnemies(
		UEnemySpawnMode* Mode,
		TArray<FSpawnParams> SpawnParams
	);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddCharacterToList(ACharacter* NewCharacter);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void InitialEnemySpawn(int Count);

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
	UEnemySpawnMode* SpawnMode = nullptr;

	void MoveEnemiesToSpawnerCountMode(TArray<AActor*> const EnemiesToMove);
};
