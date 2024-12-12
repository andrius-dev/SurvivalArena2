#pragma once
#include "GameStateInterface.h"
#include "GameFramework/GameState.h"
#include "GameStateSpawnDestruction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelCleared);

UCLASS(Blueprintable)
class TOPDOWN2_API AGameStateSpawnDestruction : public AGameStateBase, public IGameStateInterface {
	GENERATED_BODY()

public:
	virtual void HandleEnemyDefeated_Implementation(AActor* Enemy) override;
	
	virtual void HandleEnemySpawned_Implementation(AActor* Enemy) override;

	UFUNCTION(BlueprintCallable, Category="GameState")
	void HandleSpawnerDestroyed(AActor* Spawner);

	UFUNCTION(BlueprintCallable, Category="GameState")
	void SetSpawners(const TArray<AActor*>& Spawners);
	
	virtual float GetScore_Implementation() override;

	static constexpr float ScoreDestroySpawner = 300.0f;

	static constexpr float ScoreEnemyDefeated = 100.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnLevelCleared OnLevelCleared;

private:
	UPROPERTY()
	TArray<AActor*> ActiveSpawners;

	UPROPERTY()
	TArray<AActor*> ActiveEnemies;

	UFUNCTION()
	void CheckIfLevelCleared();

	float Score;
};
