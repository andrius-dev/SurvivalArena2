#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Actor.h"
#include "TopDown2/Enemies/EnemyCharacter.h"
#include "CharacterSpawner.generated.h"


class ACharacterSpawner;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
// 	FOnCharacterLoaded,
// 	AActor*, NewActor,
// 	const ACharacterSpawner*, Spawner
// );

UCLASS()
class TOPDOWN2_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()
public:	
	ACharacterSpawner();

	// UPROPERTY(BlueprintAssignable, Category="AI")
	// FOnCharacterLoaded OnCharacterLoaded;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="AI")
	bool bNoCollisionFail = false;

private:
	FName ActorName;
};
