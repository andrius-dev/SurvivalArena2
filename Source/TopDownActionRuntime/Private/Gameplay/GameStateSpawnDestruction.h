#pragma once
#include "GameStateInterface.h"
#include "GameFramework/GameState.h"
#include "GameStateSpawnDestruction.generated.h"

UCLASS(Blueprintable)
class AGameStateSpawnDestruction : public AGameStateBase, public IGameStateInterface {
	GENERATED_BODY()

public:
	// todo cost
	static constexpr float ScoreDestroySpawner = 300.0f;
	static constexpr float ScoreEnemyDefeated = 100.0f;

	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnEnemyCountChanged OnEnemyCountChanged;

	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnSpawnerCountChanged OnSpawnerCountChanged;

	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnScoreChanged OnScoreChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnLevelCleared OnLevelCleared;
	
	virtual void HandleEnemyDefeated_Implementation(AActor* Enemy) override;
	
	virtual void HandleEnemySpawned_Implementation(AActor* Enemy) override;

	UFUNCTION(BlueprintCallable, Category="GameState")
	void HandleSpawnerDestroyed(AActor* Spawner);

	UFUNCTION(BlueprintCallable, Category="GameState")
	void SetSpawners(const TArray<AActor*>& Spawners);
	
	virtual float GetScore_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActiveSpawners;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActiveEnemies;

	UFUNCTION()
	void CheckIfLevelCleared();

	float Score;
};
