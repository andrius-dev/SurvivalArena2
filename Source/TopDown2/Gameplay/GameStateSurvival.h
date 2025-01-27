#pragma once

#include "CoreMinimal.h"
#include "GameStateInterface.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateSurvival.generated.h"

UCLASS()
class TOPDOWN2_API AGameStateSurvival : public AGameStateBase, public IGameStateInterface
{
	GENERATED_BODY()

public:
	AGameStateSurvival();
	
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void HandleBeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	UFUNCTION(BlueprintCallable, Category="GameState")
	int GetRemainingEnemyCount() const;
	
	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnLevelCleared OnLevelCleared;
	
	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnScoreChanged OnScoreChanged;
	
	virtual float GetScore_Implementation() override;
	
	UFUNCTION(BlueprintPure, Category="GameState")
	FString GetFormattedScore() const;

	virtual void HandleEnemyDefeated_Implementation(AActor* Enemy) override;
	
	virtual void HandleEnemySpawned_Implementation(AActor* Enemy) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="GameState")
	int DefeatedEnemyCountToWin = 0;	

private:
	UPROPERTY(VisibleAnywhere, Category="GameState")
	int RemainingEnemyCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	int Score;
};
