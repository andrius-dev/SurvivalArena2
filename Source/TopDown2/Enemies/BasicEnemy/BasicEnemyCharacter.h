// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "BasicEnemyCharacter.generated.h"

UCLASS()
class TOPDOWN2_API ABasicEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicEnemyCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	ACharacter* PlayerCharacter;
private:
	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UBehaviorTree* DefaultBehaviorTree;

	void RunAIBehavior();
};
