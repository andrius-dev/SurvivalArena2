#include "TopDown2PlayerController.h"

#include <functional>

#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ATopDown2PlayerController::ATopDown2PlayerController() {
}

void ATopDown2PlayerController::BeginPlay() {
	Super::BeginPlay();
	const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem) {
		UE_LOG(LogTemplateCharacter, Error, TEXT("error initializing input subsystem!"));
		return;	
	}
	// todo separate function
	Subsystem->AddMappingContext(MappingContextKeyboardMouse, 1);
	auto Options = FModifyContextOptions();
	Options.bForceImmediately = true;
	Subsystem->AddMappingContext(MappingContextController, 0, Options);
}

void ATopDown2PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	// set up gameplay key bindings
}
