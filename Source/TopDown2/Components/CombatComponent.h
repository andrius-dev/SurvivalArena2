#pragma once

#include "CoreMinimal.h"
#include "WeaponMeshComponent.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

	
/**
 *
 * Declare blueprint events. 1st param is event name,
 * and the following params are four type - name pairs.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnHealthChanged,
	UCombatComponent*, CombatComponent,
	float, NewValue,
	float, OldValue,
	AActor*, Initiator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDefeat,
	UObject*, Owner 
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN2_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	// todo should be the same as collider
	const float DEFAULT_HIT_RADIUS = 20.f;
	const float DEFAULT_DAMAGE = 5.f;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void DetectMeleeHits();
	
	virtual FString GetReadableName() const override {
		return "CombatComponent";
	}

	const float DEFAULT_MAX_HEALTH = 100.f;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDefeat OnDefeat;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float Health, bool bNotify);

	/**
	 * Sets CurrentHealth to MaxHealth
	 */
	UFUNCTION(BlueprintCallable, Category="Health")
	void ResetHealth();

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCanReceiveDamage(bool NewDamage);

	UFUNCTION(BlueprintCallable, Category="Health")
	bool GetCanReceiveDamage() const;

	/**
	 * Reduces CurrentHealth by Amount and broadcasts OnHealthChanged
	 * @param Amount Raw attack value
	 * @return Actual damage taken
	 * @param Initiator Actor that attacked this component's owner
	 */
	UFUNCTION(BlueprintCallable, Category="Health")
	float TakeDamage(const float Amount, AActor* Initiator);

protected:
	UPROPERTY(EditAnywhere)
	float MaxHealth = DEFAULT_MAX_HEALTH;
	
	UPROPERTY(BlueprintReadWrite)
	float CurrentHealth = MaxHealth;

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	TObjectPtr<UWeaponMeshComponent> EquippedWeaponMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	bool bDamageActorsOfSelfClass = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	TArray<FName> TagsToIgnoreDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	float DamagePerHit = DEFAULT_DAMAGE;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	float HitRadius = DEFAULT_HIT_RADIUS;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	FLinearColor AttackTraceMissColor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	FLinearColor AttackTraceHitColor;

private:	
	UPROPERTY(VisibleAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshSocket const> BladeStart = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Combat")
	TObjectPtr<UStaticMeshSocket const> BladeEnd = nullptr;

	FVector GetSocketLocation(const UStaticMeshSocket* Socket) const;
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnoreTrace;

	UPROPERTY()
	bool bCanReceiveDamage = true;
};
