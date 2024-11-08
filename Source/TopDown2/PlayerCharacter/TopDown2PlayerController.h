#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "TopDown2PlayerController.generated.h"

/** todo use Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputAction;

// todo map controller input and calculate it here instead of doing it
// in character itself.
UCLASS()
class TOPDOWN2_API ATopDown2PlayerController : public APlayerController
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


