#include "GameModeSurvival.h"

class AGameSession;

AGameModeSurvival::AGameModeSurvival() {
	bSpawnPlayerOnStart = true;
	Score = 0;
}

int AGameModeSurvival::GetScore() const {
	return Score;
}

void AGameModeSurvival::SetScore(const int NewValue) {
	Score = NewValue;
}

FString AGameModeSurvival::GetFormattedScore() const {
	return FString::FromInt(Score);
}

APlayerController* AGameModeSurvival::SpawnPlayerController(
	ENetRole InRemoteRole,
	const FString& Options
) {
	return Super::SpawnPlayerController(InRemoteRole, Options);
}

APlayerController* AGameModeSurvival::Login(
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

void AGameModeSurvival::OnEnemyDefeated_Implementation(UObject* Enemy) {
	IEnemyDefeatedListenerInterface::OnEnemyDefeated_Implementation(Enemy);
}
