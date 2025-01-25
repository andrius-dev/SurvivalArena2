#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHealthBarInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class  IHealthBarInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetHealth(const float NewHealth, const float NewMaxHealth);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPositionInCanvas(const FVector2D& NewPosition);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetVisible(const bool NewVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSize(const FVector2D& NewSize);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UUserWidget* GetParentWidget();
};
