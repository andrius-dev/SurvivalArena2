#pragma once

#include "CoreMinimal.h"
#include "TopDown2/Enemies/EnemyGameState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "TopDown2/Enemies/EnemyCharacterInterface.h"
#include "BasicEnemyCharacter.generated.h"

UCLASS(Blueprintable, Category="AI")
class ABasicEnemyCharacter : public ACharacter, public IEnemyCharacterInterface
{
	GENERATED_BODY()

public:
	ABasicEnemyCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	
	void SetState_Implementation(const EEnemyGameState NewState) override;
	const EEnemyGameState GetState_Implementation() override;
	UCombatComponent* GetCombatComponent_Implementation() override;
	ACharacter* GetCharacter_Implementation() override;
	void BindOnDefeatedEvent_Implementation(UObject* Listener) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	EEnemyGameState CurrentState;
	
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;

	UPROPERTY()
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY()
	TScriptInterface<IEnemyDefeatedListenerInterface> OnDefeatedListener = nullptr;

	UFUNCTION()
	void DispatchOnEnemyDefeated(UCombatComponent* EnemyCombatComponent);
};

