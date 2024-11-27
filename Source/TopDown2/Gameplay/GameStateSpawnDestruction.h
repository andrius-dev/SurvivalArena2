#pragma once
#include "GameStateInterface.h"
#include "GameFramework/GameState.h"
#include "GameStateSpawnDestruction.generated.h"

UCLASS(Blueprintable)
class TOPDOWN2_API AGameStateSpawnDestruction : public AGameState, public IGameStateInterface {
	GENERATED_BODY()
public:
	virtual void HandleEnemyDefeated_Implementation(UObject* Enemy) override;

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	TArray<AActor*> ActiveSpawners;

	UPROPERTY()
	TArray<UObject*> ActiveEnemies;
};
