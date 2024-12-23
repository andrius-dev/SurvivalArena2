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
	float, NewValue,
	float, OldValue,
	AActor*, Initiator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FDefeatStarted,
	UObject*, Owner 
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN2_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	UFUNCTION(BlueprintCallable, Category="Combat")
	const TArray<FHitResult> DetectMeleeHits();
	
	virtual FString GetReadableName() const override {
		return "CombatComponent";
	}

	UPROPERTY(BlueprintAssignable, Category="Health")
	FHealth_AttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FDefeatStarted DefeatStarted;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCanReceiveDamage(bool NewDamage);

	UFUNCTION(BlueprintCallable, Category="Health")
	bool GetCanReceiveDamage() const;
	
	// todo gameplay effect? idduno
	/**
	 * Reduces CurrentHealth by Amount and broadcasts OnHealthChanged
	 * @param Amount Raw attack value
	 * @return Actual damage taken
	 * @param Initiator Actor that attacked this component's owner
	 */
	UFUNCTION(BlueprintCallable, Category="Health")
	float TakeDamage(const float Amount, AActor* Initiator);

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

	FVector GetSocketLocation(const UStaticMeshSocket* Socket) const;

	UFUNCTION()
	void InitAbilitySystem();
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnoreTrace;

	UPROPERTY()
	bool bCanReceiveDamage = true;
};
