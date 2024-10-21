#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateSurvival.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesDefeated);

/**
 * 
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
	void HandleEnemyAdded(AActor* Enemy);
	
	UFUNCTION(BlueprintCallable, Category="GameState")
	void HandleEnemyListAdded(TArray<AActor*>& Enemies, bool IsInitial);
	
	UFUNCTION(BlueprintCallable, Category="GameState")
	void HandleAllEnemiesRemoved();
	
	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnAllEnemiesDefeated OnAllEnemiesDefeated;

protected:
	virtual void BeginPlay() override;

	void AddEnemyIds(TArray<AActor*>& Enemies, const bool OnlyUnique);
	
	UPROPERTY()
	TSet<uint32> EnemyIdSet;
};
