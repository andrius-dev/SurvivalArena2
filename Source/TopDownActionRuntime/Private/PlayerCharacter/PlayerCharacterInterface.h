#pragma once

#include "CoreMinimal.h"
#include "Components/CombatComponent.h"
#include "UObject/Interface.h"
#include "PlayerCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class  IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	ACharacter* GetCharacter();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Combat")
	UCombatComponent* GetCombatComponent();
};
