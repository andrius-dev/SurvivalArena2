#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

UCLASS()
class  ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABaseEnemyController();
	
	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TSoftClassPtr<UClass>> DamageablePawnTypes;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AI|Components|PawnSensing")
	bool CanDamagePawn(APawn* InPawn);

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
};
