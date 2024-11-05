#pragma once
#include "EnemyDefeatedListener.h"
#include "EnemyGameState.h"
#include "TopDown2/Components/CombatComponent.h"
#include "TopDown2/Components/HealthComponent.h"
#include "EnemyCharacter.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEnemyCharacter : public UInterface  {
public:
	GENERATED_BODY()
};

class TOPDOWN2_API IEnemyCharacter {
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	void BindOnDefeatedEvent(const TScriptInterface<IEnemyDefeatedListener>& Listener);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	void SetState(EEnemyGameState State);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	const EEnemyGameState GetState();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI")
	ACharacter* GetCharacter();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Health")
	UHealthComponent* GetHealthComponent();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI|Combat")
	UCombatComponent* GetCombatComponent();
};

