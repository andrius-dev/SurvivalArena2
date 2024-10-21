#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Actor.h"
#include "CharacterSpawner.generated.h"


class ACharacterSpawner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCharacterLoaded,
	AActor*, NewActor,
	const ACharacterSpawner*, Spawner
);

UCLASS()
class TOPDOWN2_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()
public:	
	ACharacterSpawner();

	UPROPERTY(BlueprintAssignable, Category="AI")
	FOnCharacterLoaded OnCharacterLoaded;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* LoadCharacterWithBehaviorTree(UBehaviorTree* BehaviorTree);
	
	TSubclassOf<ACharacter> GetSpawnedClass() {
		return SpawnedClass;
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<ACharacter> SpawnedClass;

	UPROPERTY(BlueprintReadWrite, Category="AI")
	bool bNoCollisionFail = false;

private:
	FName ActorName;
};
