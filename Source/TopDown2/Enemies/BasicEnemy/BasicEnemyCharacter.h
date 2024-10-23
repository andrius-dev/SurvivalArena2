#pragma once

#include "CoreMinimal.h"
#include "EEnemyGameState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "BasicEnemyCharacter.generated.h"

UCLASS()
class TOPDOWN2_API ABasicEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABasicEnemyCharacter();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetState(EEnemyGameState NewState);
	
	UFUNCTION(BlueprintCallable, Category="AI")
	EEnemyGameState GetState() const;

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ACharacter* PlayerCharacter;
	
private:
	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UBehaviorTree* DefaultBehaviorTree = nullptr;
	
	UPROPERTY()
	EEnemyGameState CurrentState;
	
	UPROPERTY()
	APawn* CachedPawn = nullptr;
};

