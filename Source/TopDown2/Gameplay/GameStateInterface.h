#pragma once
#include "GameStateInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameStateInterface : public UInterface {
	GENERATED_BODY()
};

class TOPDOWN2_API IGameStateInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameState")
	void HandleEnemyDefeated(AActor* Enemy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameState")
	void HandleEnemySpawned(AActor* Enemy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GameState")
	float GetScore();
};
