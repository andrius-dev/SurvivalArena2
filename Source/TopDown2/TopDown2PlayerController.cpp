// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDown2PlayerController.h"
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
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()
	)) {
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ATopDown2PlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();
}
