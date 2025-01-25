#pragma once

#pragma once
 
#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "HeroAbilityTask_OnTick.generated.h"
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);
 
/**
 * Task for abilities that supply tick and its' delta time.
 */
UCLASS()
class UHeroAbilityTask_OnTick : public UAbilityTask
{
	GENERATED_BODY()
 
	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;
 
public:
 
	UHeroAbilityTask_OnTick(const FObjectInitializer& ObjectInitializer);
 
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UHeroAbilityTask_OnTick* AbilityTaskOnTick(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName
	);
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
};
