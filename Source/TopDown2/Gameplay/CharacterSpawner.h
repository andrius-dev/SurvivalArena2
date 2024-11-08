#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
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

private:
	FName ActorName;
};
