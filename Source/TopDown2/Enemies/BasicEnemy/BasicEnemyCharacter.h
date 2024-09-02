#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEnemyCharacter.generated.h"

UCLASS()
class TOPDOWN2_API ABasicEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
