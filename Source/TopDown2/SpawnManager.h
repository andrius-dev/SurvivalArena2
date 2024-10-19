#pragma once

#include <new>
#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class ACharacterSpawner;

UCLASS()
class TOPDOWN2_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddCharacterToList(ACharacter* NewCharacter);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MoveEnemiesToSpawners(TArray<AActor*> const EnemiesToMove);

	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<AActor*> LoadEnemiesAtStartLocation(UBehaviorTree* BehaviorTree);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<AActor*> GetCharactersPool();
	
private:
	UPROPERTY()
	TArray<AActor*> CharactersPool;
	
	UPROPERTY()
	TArray<ACharacterSpawner*> SpawnersList;
};
