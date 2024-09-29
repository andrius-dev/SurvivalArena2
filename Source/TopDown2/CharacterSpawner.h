// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSpawner.generated.h"

UCLASS()
class TOPDOWN2_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSpawner();

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* SpawnCharacter() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<ACharacter> SpawnedClass;

private:
	FName ActorName;
};
