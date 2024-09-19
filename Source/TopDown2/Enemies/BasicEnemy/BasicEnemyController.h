#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BasicEnemyController.generated.h"

UCLASS()
class TOPDOWN2_API ABasicEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABasicEnemyController();
	
	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<UClass*> KickableAssTypes;
	
	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UBehaviorTree* BaseBehaviourTree;
	
	void RunAIBehaviors(UBehaviorTree* BehaviorTree, UWorld* World, ACharacter* PlayerCharacter);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AI|Components|PawnSensing")
	bool IsKickableAss(APawn* InPawn);

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	void LogPawnAction(const FString& PawnAction, const FString& Name);
};
