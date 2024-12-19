#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "TopDown2/Enemies/EnemyGameState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "TopDown2/Enemies/EnemyCharacterInterface.h"
#include "BaseEnemyCharacter.generated.h"

class UCombatAttributeSet;
class UAbilitySystemComponent;

UCLASS(Blueprintable, Category="AI")
class ABaseEnemyCharacter : public ACharacter, public IAbilitySystemInterface, public IEnemyCharacterInterface
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
	
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AbilitySystem)
	TObjectPtr<UCombatAttributeSet> CharacterAttributeSet;

	UFUNCTION(BlueprintCallable)
	void InitAttributes();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AbilitySystem, meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY()
	EEnemyGameState CurrentState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;

	UPROPERTY()
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor = nullptr;
};

