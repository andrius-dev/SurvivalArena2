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
	// Call the base class  
	Super::BeginPlay();
	//Add Input Mapping Context
	const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem) {
		// todo add new logging function
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
	// set up gameplay key bindings
	Super::SetupInputComponent();
}
