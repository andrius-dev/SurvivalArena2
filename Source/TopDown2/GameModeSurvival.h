#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "CharacterSpawner.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"
#include "PlayerCharacter/TopDown2Character.h"
#include "GameModeSurvival.generated.h"

class AGameSession;

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
 */
UCLASS()
class TOPDOWN2_API AGameModeSurvival : public AGameModeBase {
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
