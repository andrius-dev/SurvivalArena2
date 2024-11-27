#include "BaseGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "TopDown2/Util/Log.h"

class AGameSession;

ABaseGameMode::ABaseGameMode() {
	bSpawnPlayerOnStart = true;

	if (!SpawnManager) {
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
		return;
	}
}

void ABaseGameMode::BeginPlay() {
	Super::BeginPlay();

	const auto SpawnActor =
		UGameplayStatics::GetActorOfClass(this, ASpawnManager::StaticClass());
	verify(SpawnActor);
	SpawnManager = CastChecked<ASpawnManager>(SpawnActor);
	const auto PooledEnemies = SpawnManager->InitEnemyPool(EnemiesToSpawn);
	UE_LOG(LogTopDown2, All, TEXT("Initializing %s enemies"), EnemiesToSpawn.Num());

	for (const auto Enemy : PooledEnemies) {
		IEnemyCharacterInterface::Execute_GetCombatComponent(Enemy)
			->OnDefeat.AddUniqueDynamic(this, &ABaseGameMode::OnEnemyDefeated);
	}
}

APlayerController* ABaseGameMode::SpawnPlayerController(
	ENetRole InRemoteRole,
	const FString& Options
) {
	return Super::SpawnPlayerController(InRemoteRole, Options);
}

APlayerController* ABaseGameMode::Login(
	UPlayer* NewPlayer,
	ENetRole InRemoteRole,
	const FString& Portal,
	const FString& Options,
	const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage
) {
	if (bSpawnPlayerOnStart) {
		return Super::Login(
			NewPlayer,
			InRemoteRole,
			Portal,
			Options,
			UniqueId,
			ErrorMessage
		);
	}

	const auto TempGameSession = GameSession;
	if (TempGameSession == nullptr) {
		ErrorMessage = TEXT(
			"Failed to spawn player controller, GameSession is null"
		);
		return nullptr;
	}

	ErrorMessage = TempGameSession->ApproveLogin(Options);
	if (!ErrorMessage.IsEmpty()) {
		return nullptr;
	}

	APlayerController* const NewPlayerController = SpawnPlayerController(
		InRemoteRole,
		Options
	);
	if (NewPlayerController == nullptr) {
		// Handle spawn failure.
		UE_LOG(
			LogGameMode,
			Log,
			TEXT("Login: Couldn't spawn player controller of class %s"),
			PlayerControllerClass ? *PlayerControllerClass->GetName() : TEXT("NULL")
		);
		ErrorMessage = FString::Printf(TEXT("Failed to spawn player controller"));
		return nullptr;
	}

	// Customize incoming player based on URL options
	ErrorMessage = InitNewPlayer(
		NewPlayerController,
		UniqueId,
		Options,
		Portal
	);
	if (!ErrorMessage.IsEmpty()) {
		NewPlayerController->Destroy();
		return nullptr;
	}

	return NewPlayerController;
}

void ABaseGameMode::OnEnemyDefeated(UObject* Enemy) {
	if (GameState->Implements<UGameStateInterface>()) {
		IGameStateInterface::Execute_HandleEnemyDefeated(GameState, Enemy);
	}
}
