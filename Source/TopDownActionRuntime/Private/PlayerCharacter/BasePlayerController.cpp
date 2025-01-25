#include "BasePlayerController.h"

#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Util/Log.h"

ABasePlayerController::ABasePlayerController() {
}

void ABasePlayerController::BeginPlay() {
	Super::BeginPlay();
	const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!InputSubsystem) {
		UE_LOG(LogTopDown2, Error, TEXT("error initializing input subsystem!"));
		return;	
	}
	InputSubsystem->AddMappingContext(MappingContextKeyboardMouse, 1);
	auto Options = FModifyContextOptions();
	Options.bForceImmediately = true;
	InputSubsystem->AddMappingContext(MappingContextController, 0, Options);
}

void ABasePlayerController::BeginPlayingState() {
	Super::BeginPlayingState();
}

void ABasePlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
}
