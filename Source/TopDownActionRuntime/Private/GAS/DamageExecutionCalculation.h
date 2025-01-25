#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecutionCalculation.generated.h"

struct FDamageStatics;
/**
 * Calculates damage taken
 */
UCLASS()
class  UDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalculation();
	
	static FDamageStatics& GetDamageStatics();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};
