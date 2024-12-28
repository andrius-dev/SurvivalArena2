#pragma once

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_SixParams(
	FAttributeEvent,
	AActor*,					// Instigator
	AActor*,					// Causer
	const FGameplayEffectSpec*,  // Effect spec
	float,						// Effect magnitude	
	float,						// Old value
	float						// New value
)
