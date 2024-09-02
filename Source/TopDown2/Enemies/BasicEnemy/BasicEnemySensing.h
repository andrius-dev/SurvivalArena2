// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BasicEnemySensing.generated.h"

UCLASS(Category=AI)
class TOPDOWN2_API UBasicEnemySensing : public UPawnSensingComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<UClass*> KickableAssTypes;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AI|Components|PawnSensing")
	void MoveToAss(AActor* Target);
	virtual void BroadcastOnSeePawn(APawn& Pawn) override;
	bool IsKickableAss(const APawn& Pawn);
private:
	void LogPawnAction(const FString& PawnAction, const FString& Name);
	UPROPERTY()
	AAIController* SelfAIController;

public:
};
