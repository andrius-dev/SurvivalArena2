#include "CharacterSpawner.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

ACharacterSpawner::ACharacterSpawner() {
	PrimaryActorTick.bCanEverTick = false;
	ActorName = "DefaultName";
}

void ACharacterSpawner::BeginPlay() {
	Super::BeginPlay();
}
