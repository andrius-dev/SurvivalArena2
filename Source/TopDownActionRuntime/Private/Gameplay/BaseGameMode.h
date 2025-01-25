#pragma once

#include "CoreMinimal.h"
#include "SpawnManager.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerCharacter/BasePlayerCharacter.h"
#include "BaseGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnEnemyDefeated,
	TScriptInterface<IEnemyCharacterInterface>, Enemy
);

/**
 * Game loop description to be added here
 * Online feature - game master. He/she will control how enemies spawn
 */
UCLASS()
class  ABaseGameMode : public AGameModeBase {
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

	// todo move to game state
	UPROPERTY(BlueprintAssignable, Category="GameMode")
	FOnEnemyDefeated OnEnemyDefeated;
	virtual AActor* FindPlayerStart_Implementation(
		AController* Player,
		const FString& IncomingName
	) override;

protected:
	UPROPERTY(EditAnywhere, Category="GameMode")
	bool bSpawnPlayerOnStart = true;

	UFUNCTION(BlueprintCallable, Category="GameMode")
	ASpawnManager* GetSpawnManager();

	/**
	 * Creates enemy characters, registers events and places them in object pool
	 * @param EnemiesToSpawn 
	 */
	UFUNCTION(BlueprintCallable, Category="GameMode")
	void InitEnemies(const TArray<FSpawnParams> EnemiesToSpawn);

	UFUNCTION(BlueprintCallable, Category="GameMode")
	virtual void HandleEnemyDefeated(UObject* Enemy);
private:
	UPROPERTY()
	TObjectPtr<ABasePlayerCharacter> PlayerCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<ASpawnManager> SpawnManager = nullptr;
};
