#pragma once
#include "EnemyDefeatedListener.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEnemyDefeatedListener : public UInterface {
	GENERATED_BODY()
};

class TOPDOWN2_API IEnemyDefeatedListener {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AI")
	void OnEnemyDefeated(ACharacter* Enemy);
};
