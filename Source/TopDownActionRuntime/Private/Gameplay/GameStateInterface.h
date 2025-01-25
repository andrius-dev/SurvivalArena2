#pragma once
#include "GameStateInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelCleared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnEnemyCountChanged,
	int, OldValue,
	int, NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnSpawnerCountChanged,
	int, OldValue,
	int, NewValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnScoreChanged,
	int, NewValue,
	int, OldValue
);

UINTERFACE(MinimalAPI, Blueprintable)
class UGameStateInterface : public UInterface {
	GENERATED_BODY()
};

class  IGameStateInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameState")
	void HandleEnemyDefeated(AActor* Enemy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameState")
	void HandleEnemySpawned(AActor* Enemy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameState")
	float GetScore();
};
