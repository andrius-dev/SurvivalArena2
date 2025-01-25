#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UNiagaraSystem;
class UInputAction;

UCLASS()
class ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MappingContextKeyboardMouse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MappingContextController;

protected:
	virtual void BeginPlayingState() override;

	virtual void SetupInputComponent() override;
	// To add mapping context
	virtual void BeginPlay();
};
