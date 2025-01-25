#pragma once

#include "CoreMinimal.h"
#include "SpawnerType.h"
#include "GameFramework/Actor.h"
#include "Enemies/EnemyCharacterInterface.h"
#include "CharacterSpawner.generated.h"


class ACharacterSpawner;

/**
 * todo move blueprint functions here
 */
UCLASS()
class  ACharacterSpawner : public AActor
{
	GENERATED_BODY()
public:	
	ACharacterSpawner();

protected:
	virtual void BeginPlay() override;

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
