#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "AimArrowComponent.generated.h"

/**
 * Arrow that is used to aim attacks and projectiles
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class  UAimArrowComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AimToTarget(const FVector& Target);
	
};
