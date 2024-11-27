#pragma once

#include "CoreMinimal.h"
#include "GameStateInterface.h"
#include "SpawnManager.h"
#include "GameFramework/GameModeBase.h"
#include "TopDown2/PlayerCharacter/TopDown2Character.h"
#include "GameFramework/GameSession.h"
#include "TopDown2/Enemies/EnemyDefeatedListenerInterface.h"
#include "BaseGameMode.generated.h"


/**
 * Game loop description to be added here
 * Online feature - game master. He/she will control how enemies spawn
 */
UCLASS()
class TOPDOWN2_API ABaseGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	ABaseGameMode();

protected:
	virtual void BeginPlay() override;

public:
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

	UFUNCTION()
	virtual void OnEnemyDefeated(UObject* Enemy);

protected:
	UPROPERTY(EditAnywhere, Category="GameMode")
	bool bSpawnPlayerOnStart = true;

	UPROPERTY(EditAnywhere, Category="GameMode")
	TArray<FSpawnParams> EnemiesToSpawn;

private:
	UPROPERTY()
	TObjectPtr<ATopDown2Character> PlayerCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<ASpawnManager> SpawnManager = nullptr;
};
