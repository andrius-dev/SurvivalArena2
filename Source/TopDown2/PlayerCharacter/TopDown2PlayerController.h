// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "TopDown2PlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTopDown2, Log, All);

UCLASS()
class ATopDown2PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDown2PlayerController();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContextKeyboardMouse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContextController;
protected:
	virtual void SetupInputComponent() override;
	// To add mapping context
	virtual void BeginPlay();
};


