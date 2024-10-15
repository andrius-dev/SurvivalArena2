#pragma once

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
	void MoveEnemiesToSpawners();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void LoadEnemiesAtStartLocation(UBehaviorTree* BehaviorTree);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, Category="AI")
	TArray<AActor*> CharactersPool;
	
	UPROPERTY(BlueprintReadOnly, Category="AI")
	TArray<ACharacterSpawner*> SpawnersList;
};
