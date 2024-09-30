#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSpawner.generated.h"

UCLASS()
class TOPDOWN2_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACharacterSpawner();

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* SpawnCharacter() const;
	
	TSubclassOf<ACharacter> GetSpawnedClass() {
		return SpawnedClass;
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<ACharacter> SpawnedClass;

private:
	FName ActorName;
};
