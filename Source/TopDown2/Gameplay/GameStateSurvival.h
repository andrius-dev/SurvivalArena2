#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateSurvival.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesDefeated);

/**
 * todo move score here
 */
UCLASS()
class TOPDOWN2_API AGameStateSurvival : public AGameStateBase
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

	UFUNCTION(BlueprintCallable, Category="GameState")
	void HandleEnemyDefeated(AActor* Enemy);
	
	UFUNCTION(BlueprintCallable, Category="GameState")
	void HandleAllEnemiesRemoved();
	
	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnAllEnemiesDefeated OnAllEnemiesDefeated;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="GameState")
	int DefeatedEnemyCountToWin = 0;	

private:
	UPROPERTY(VisibleAnywhere, Category="GameState")
	int RemainingEnemyCount = 0;	
};
