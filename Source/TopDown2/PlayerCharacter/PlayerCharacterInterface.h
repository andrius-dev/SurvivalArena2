#pragma once

#include "CoreMinimal.h"
#include "TopDown2/Components/CombatComponent.h"
#include "UObject/Interface.h"
#include "PlayerCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class TOPDOWN2_API IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	ACharacter* GetCharacter();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Combat")
	UCombatComponent* GetCombatComponent();
};
