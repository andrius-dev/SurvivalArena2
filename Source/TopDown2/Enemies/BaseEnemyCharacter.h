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

// todo: add a spawnable actor interface/component for enemies, bullets, etc.

UCLASS(Blueprintable, Category="AI")
class ABaseEnemyCharacter :
	public ACharacter,
	public IEnemyCharacterInterface,
	public IAbilitySystemInterface
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
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AbilitySystem")
	TObjectPtr<UCombatAttributeSet> CombatAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AbilitySystem")
	TObjectPtr<UGameplayEffect> GameplayEffect_InitAttributes;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InitAttributes();

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystem = nullptr;
	
	UPROPERTY()
	EEnemyGameState CurrentState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent = nullptr;

	UPROPERTY()
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor = nullptr;
};

