#include "HeroAbilityTask_OnTick.h"
 
 
#include "HeroAbilityTask_OnTick.h"
 
UHeroAbilityTask_OnTick::UHeroAbilityTask_OnTick(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}
 
UHeroAbilityTask_OnTick* UHeroAbilityTask_OnTick::AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UHeroAbilityTask_OnTick* MyObj = NewAbilityTask<UHeroAbilityTask_OnTick>(OwningAbility, TaskInstanceName);
	return MyObj;
}
 
void UHeroAbilityTask_OnTick::Activate()
{
	Super::Activate();
}
 
void UHeroAbilityTask_OnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}
