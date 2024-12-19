#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthBarInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHealthBarInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOPDOWN2_API IHealthBarInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMaxHealth(const float NewMaxHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCurrentHealth(const float NewCurrentHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Health")
	void DecreaseHealth(const float DeltaHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncreaseHealth(const float DeltaHealth);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPositionInCanvas(const FVector2D& NewPosition);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetVisible(const bool NewVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSize(const FVector2D& NewSize);
};
