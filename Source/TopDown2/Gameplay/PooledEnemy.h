#pragma once
#include "PooledEnemy.generated.h"

USTRUCT(BlueprintType)
struct FPooledEnemy {
	GENERATED_BODY()

	UPROPERTY()
	AActor* Character;
	
	UPROPERTY()
	bool bActive;
};
