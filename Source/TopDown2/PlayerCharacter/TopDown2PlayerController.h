#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "TopDown2PlayerController.generated.h"

class UNiagaraSystem;
class UInputAction;

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


