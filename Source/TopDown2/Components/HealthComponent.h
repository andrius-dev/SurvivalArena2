#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthComponent;

/**
 * Declare blueprint events. 1st param is event name,
 * and the following params are four type - name pairs.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnHealthChanged,
	UHealthComponent*, HealthComponent,
	float, NewValue,
	float, OldValue,
	AActor*, Initiator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDeath,
	UHealthComponent*, HealthComponent
);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class TOPDOWN2_API UHealthComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UHealthComponent();
	
	const float DEFAULT_MAX_HEALTH = 100.f;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDeath OnDeath;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category="Health")
	static UHealthComponent* FindHealthComponent(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth();

	/**
	 * @param Amount Raw attack value
	 * @return Actual damage taken
	 */
	UFUNCTION(BlueprintCallable, Category="Health")
	float TakeDamage(const float Amount);

protected:
	UPROPERTY(BlueprintReadOnly)
	float MaxHealth = DEFAULT_MAX_HEALTH;
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = MaxHealth;
	
	virtual void BeginPlay() override;
};
