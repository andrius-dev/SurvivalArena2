#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAnimationInputs.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterAnimationInputs : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOPDOWN2_API ICharacterAnimationInputs
{
	GENERATED_BODY()

	// todo clean-up, not needed anymore
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsDodgePressed();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsAttackPressed();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int AttackInputCount();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void ResetAttackInputChain();
};
