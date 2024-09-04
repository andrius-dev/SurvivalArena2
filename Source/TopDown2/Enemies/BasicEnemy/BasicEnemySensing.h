// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BasicEnemySensing.generated.h"

UCLASS(ClassGroup=AI, HideCategories=(Activation, "Components|Activation", Collision), meta=(BlueprintSpawnableComponent))
class TOPDOWN2_API UBasicEnemySensing : public UPawnSensingComponent
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSeePawnDelegate, APawn*, Pawn );
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FHearNoiseDelegate, APawn*, Instigator, const FVector&, Location, float, Volume);
	
public:
	virtual void BeginPlay() override;
	
protected:
private:
	UPROPERTY()
	AAIController* SelfAIController;

public:
};
