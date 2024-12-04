#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CombatCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Attributes for characters that receive and/or deal damage 
 */
UCLASS()
class TOPDOWN2_API UCombatCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCombatCharacterAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCombatCharacterAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")
	FGameplayAttributeData BaseDefence;
	ATTRIBUTE_ACCESSORS(UCombatCharacterAttributeSet, BaseDefence);
	
	UPROPERTY(BlueprintReadOnly, Category="Combat|Attributes")	
	FGameplayAttributeData BaseAttack;
	ATTRIBUTE_ACCESSORS(UCombatCharacterAttributeSet, BaseAttack);
	
	UCombatCharacterAttributeSet();
	
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
};
