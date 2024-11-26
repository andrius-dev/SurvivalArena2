#pragma once

#include "CoreMinimal.h"
#include "SpawnerType.h"
#include "GameFramework/Actor.h"
#include "TopDown2/Enemies/EnemyCharacterInterface.h"
#include "CharacterSpawner.generated.h"


class ACharacterSpawner;

UCLASS()
class TOPDOWN2_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()
public:	
	ACharacterSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="AI")
	bool bNoCollisionFail = false;

	// todo not sure about this one
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TSubclassOf<UEnemyCharacterInterface> DefaultSpawnedClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	ESpawnerType EnemySpawnFlow;

	/**
	 * Delays first spawn when SpawnerType is automatic
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	double AutoSpawnDelayOnStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	double AutoSpawnDelay;

private:
	FName ActorName;
};
