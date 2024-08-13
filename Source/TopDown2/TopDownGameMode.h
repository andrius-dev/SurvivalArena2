// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN2_API ATopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Score")
	int GetScore() const;
	UFUNCTION(BlueprintCallable, Category="Score")
	void SetScore(const int Score);
	std::string GetFormattedScore() const;
	
private:
	int score;
};
