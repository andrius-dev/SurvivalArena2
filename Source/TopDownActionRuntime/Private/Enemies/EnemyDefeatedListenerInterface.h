#pragma once
#include "EnemyDefeatedListenerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEnemyDefeatedListenerInterface : public UInterface {
	GENERATED_BODY()
};

class  IEnemyDefeatedListenerInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AI")
	void OnEnemyDefeated(UObject* Enemy);
};
