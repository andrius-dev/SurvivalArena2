// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthComponent;

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

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent) )
class TOPDOWN2_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category="Health")
	static UHealthComponent* FindHealthComponent(const AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth();

	/**
	 * 
	 * @param Amount Raw attack value
	 * @return Actual damage taken
	 */
	UFUNCTION(BlueprintCallable, Category="Health")
	float TakeDamage(const float Amount);

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDeath OnDeath;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	float CurrentHealth;
	const float MaxHealth = 100.f;
};
