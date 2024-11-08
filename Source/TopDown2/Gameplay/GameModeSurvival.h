#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDown2/PlayerCharacter/TopDown2Character.h"
#include "GameFramework/GameSession.h"
#include "TopDown2/Enemies/EnemyDefeatedListenerInterface.h"
#include "GameModeSurvival.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnScoreChanged,
	AGameMode*,
	GameMode,
	int,
	NewValue,
	int,
	OldValue
);

/**
 * Game loop description to be added here
 * Online feature - game master. He/she will control how enemies spawn
 */
UCLASS()
class TOPDOWN2_API AGameModeSurvival : public AGameModeBase, public IEnemyDefeatedListenerInterface {
	GENERATED_BODY()

public:
	AGameModeSurvival();

	UPROPERTY(BlueprintAssignable, Category="Score")
	FOnScoreChanged OnScoreChanged;

	UFUNCTION(BlueprintCallable, Category="Score")
	int GetScore() const;

	UFUNCTION(BlueprintPure, Category="Score")
	FString GetFormattedScore() const;

	virtual APlayerController* SpawnPlayerController(
		ENetRole InRemoteRole,
		const FString& Options
	) override;

	virtual APlayerController* Login(
		UPlayer* NewPlayer,
		ENetRole InRemoteRole,
		const FString& Portal,
		const FString& Options,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage
	) override;

	virtual void OnEnemyDefeated_Implementation(UObject* Enemy) override;

protected:
	UFUNCTION(BlueprintCallable, Category="Score")
	void SetScore(const int Score);

	UPROPERTY(EditAnywhere, Category="GameMode")
	bool bSpawnPlayerOnStart = true;

private:
	UPROPERTY(VisibleAnywhere)
	int Score;

	UPROPERTY(VisibleAnywhere, Category="GameMode")
	TObjectPtr<ATopDown2Character> PlayerCharacter = nullptr;
};
