#pragma once

#include "CoreMinimal.h"
#include "TopDown2/Enemies/EnemyGameState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "TopDown2/Enemies/EnemyCharacterInterface.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS(Blueprintable, Category="AI")
class ABaseEnemyCharacter : public ACharacter, public IEnemyCharacterInterface
{
	GENERATED_BODY()

public:
	ABaseEnemyCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void EventSpawned_Implementation(AActor* Spawner) override;
	virtual void SetState_Implementation(const EEnemyGameState NewState) override;
	virtual const EEnemyGameState GetState_Implementation() override;
	virtual UCombatComponent* GetCombatComponent_Implementation() override;
	virtual ACharacter* GetCharacter_Implementation() override;
	
	virtual AActor* GetTargetActor_Implementation() override;
	virtual void SetTargetActor_Implementation(AActor* NewTargetActor) override;

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
	TObjectPtr<AActor> TargetActor = nullptr;
};

