#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "BasicEnemyController.h"

ABasicEnemyCharacter::ABasicEnemyCharacter() {
	CurrentState = EEnemyGameState::Inactive;
}

void ABasicEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	AIControllerClass = ABasicEnemyController::StaticClass();

	if (DefaultBehaviorTree == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("DefaultBehaviorTree is null"));
	}
}

void ABasicEnemyCharacter::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);
}

EEnemyGameState ABasicEnemyCharacter::GetState() const {
	return CurrentState;
}

void ABasicEnemyCharacter::SetState(const EEnemyGameState NewState) {
	CurrentState = NewState;
	bool bHidden;
	bool bEnableCollision;

	switch (CurrentState) {
	case EEnemyGameState::Active:
		bHidden = false;
		bEnableCollision = true;
		GetController()->Possess(this);
		break;
	case EEnemyGameState::Inactive:
		bHidden = true;
		bEnableCollision = false;
		GetController()->UnPossess();
		break;
	default: ;
		bHidden = true;
		bEnableCollision = false;
	}

	SetActorHiddenInGame(bHidden);
	SetActorEnableCollision(bEnableCollision);
}
