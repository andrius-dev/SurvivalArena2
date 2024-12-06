#pragma once
#include "EnemyDefeatedListenerInterface.h"
#include "EnemyGameState.h"
#include "TopDown2/Components/CombatComponent.h"
#include "EnemyCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEnemyCharacterInterface : public UInterface {
	GENERATED_BODY()
};

class TOPDOWN2_API IEnemyCharacterInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	void SetState(EEnemyGameState State);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	const EEnemyGameState GetState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	ACharacter* GetCharacter();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Combat")
	UCombatComponent* GetCombatComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Combat")
	void EventSpawned(AActor* Spawner);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Combat")
	void SetTargetActor(AActor* NewTargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Combat")
	AActor* GetTargetActor();
};
