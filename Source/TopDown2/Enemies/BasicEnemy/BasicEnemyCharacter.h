#pragma once

#include "CoreMinimal.h"
#include "TopDown2/Enemies/EnemyGameState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "TopDown2/Enemies/EnemyCharacter.h"
#include "BasicEnemyCharacter.generated.h"

UCLASS(Blueprintable, Category="AI")
class TOPDOWN2_API ABasicEnemyCharacter : public ACharacter, public IEnemyCharacter
{
	GENERATED_BODY()

public:
	ABasicEnemyCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void SetState_Implementation(const EEnemyGameState NewState) override;
	virtual const EEnemyGameState GetState_Implementation() override;
	virtual UCombatComponent* GetCombatComponent_Implementation() override;
	virtual ACharacter* GetCharacter_Implementation() override;
	virtual UHealthComponent* GetHealthComponent_Implementation() override;
	
	virtual void
	BindOnDefeatedEvent_Implementation(const TScriptInterface<IEnemyDefeatedListener>& Listener) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	EEnemyGameState CurrentState;
	
	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;

	UPROPERTY()
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY()
	TScriptInterface<IEnemyDefeatedListener> OnDefeatedListener = nullptr;

	UFUNCTION()
	void DispatchOnEnemyDefeated(UHealthComponent* EnemyHealthComponent);
};

