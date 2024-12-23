#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

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
};
