#pragma once

#include "CoreMinimal.h"
#include "HealthBarInterface.h"
#include "WeaponMeshComponent.h"
#include "Components/ActorComponent.h"
#include "TopDown2/GAS/CombatAttributeSet.h"
#include "CombatComponent.generated.h"
	
/**
 *
 * Declare blueprint events. 1st param is event name,
 * and the following params are four type - name pairs.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FHealth_AttributeChanged,
	UCombatComponent*, CombatComponent,
	float, OldValue,
	float, NewValue,
	AActor*, Initiator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDefeatStarted,
	UObject*, Owner 
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDefeatEnded,
	UObject*, Owner 
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnDetectedMeleeHit,
	UObject*, Owner,
	const TArray<FHitResult>, HitResults
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN2_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	// todo return combat components or something?
	UFUNCTION(BlueprintCallable, Category="Combat")
	const TArray<AActor*> DetectMeleeHits(const TArray<AActor*>& ActorsToIgnore);
	
	virtual FString GetReadableName() const override {
		return "CombatComponent";
	}

	UPROPERTY(BlueprintAssignable, Category="Health")
	mutable FHealth_AttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Combat")
	mutable FOnDefeatStarted OnDefeatStarted;
	
	UPROPERTY(BlueprintAssignable, Category="Combat")
	mutable FOnDefeatEnded OnDefeatEnded;

	// todo: needs to be in melee component or something
	UPROPERTY(BlueprintAssignable, Category="Combat")
	mutable FOnDetectedMeleeHit OnDetectedMeleeHit;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCanReceiveDamage(bool NewDamage);

	UFUNCTION(BlueprintCallable, Category="Health")
	bool GetCanReceiveDamage() const;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	TObjectPtr<UWeaponMeshComponent> EquippedWeaponMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TScriptInterface<IHealthBarInterface> HealthWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	bool bDamageActorsOfSelfClass = true;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Combat")
	TArray<FName> TagsToIgnoreDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	FLinearColor AttackTraceMissColor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	FLinearColor AttackTraceHitColor;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Combat")
	TObjectPtr<const UCombatAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Combat")
	TObjectPtr<const UAbilitySystemComponent> AbilitySystemComponent;

	virtual void BeginPlay() override;

private:
	// todo separate to a weapon component
	UPROPERTY(VisibleAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshSocket const> BladeStart = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshSocket const> BladeEnd = nullptr;
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnoreTrace;

	UPROPERTY()
	bool bCanReceiveDamage = true;

	void HandleHealthChanged(
		AActor* Instigator,
		AActor* DamageCauser,
		const FGameplayEffectSpec* DamageEffectSpec,
		float DamageMagnitude,
		float OldValue,
		float NewValue
	);

	UFUNCTION()
	void HandleDefeatStarted();

	UFUNCTION()
	void HandleDefeatEnded();

	FVector GetSocketLocation(const UStaticMeshSocket* Socket) const;

	UFUNCTION()
	void InitAbilitySystem();
};
