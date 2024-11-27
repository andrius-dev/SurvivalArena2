#include "TopDown2PlayerController.h"

#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TopDown2/TopDown2.h"
#include "TopDown2/Util/Log.h"

ATopDown2PlayerController::ATopDown2PlayerController() {
}

void ATopDown2PlayerController::BeginPlay() {
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

void ATopDown2PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
}
