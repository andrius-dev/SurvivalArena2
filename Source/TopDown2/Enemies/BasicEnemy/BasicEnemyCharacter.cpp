#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "BasicEnemyController.h"

ABasicEnemyCharacter::ABasicEnemyCharacter()
{
}

void ABasicEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AIControllerClass = ABasicEnemyController::StaticClass();
	
	if (DefaultBehaviorTree == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("DefaultBehaviorTree is null"));
	}
}

void ABasicEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
