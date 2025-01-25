#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "BaseAttributeSet.h"
#include "CombatAttributeSet.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepleted);

/**
 * Attributes for characters that receive and/or deal damage 
 */
UCLASS()
class TOPDOWN2_API UCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")
	FGameplayAttributeData BaseDefence;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, BaseDefence);
	
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")	
	FGameplayAttributeData BaseAttack;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, BaseAttack);

	mutable FAttributeEvent OnHealthChanged;

	mutable FAttributeEvent OnMaxHealthChanged;

	mutable FOnHealthDepleted OnHealthDepleted;
	
	UCombatAttributeSet();
	
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	
	virtual void PostGameplayEffectExecute(
		const FGameplayEffectModCallbackData& Data
	) override;
	
	virtual void PreAttributeChange(
		const FGameplayAttribute& Attribute,
		float& NewValue
	) override;
	
	virtual void PostAttributeChange(
		const FGameplayAttribute& Attribute,
		float OldValue,
		float NewValue
	) override;
	
	virtual void InitFromMetaDataTable(const UDataTable* DataTable) override;

	/**
	 * Sets CurrentHealth to MaxHealth
	 */
	void ResetHealth();


private:
	UFUNCTION()
	void LogAttributeValue(const FGameplayAttribute& Attribute, const float Value) const;
	
	UFUNCTION()
	void HandleAttributeChange(
		const FGameplayAttribute& Attribute,
		const float OldValue,
		const float NewValue
    );
};
