#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "BasicEnemyController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDown2/Util/Log.h"

ABasicEnemyCharacter::ABasicEnemyCharacter() {
	CurrentState = EEnemyGameState::Inactive;
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	AIControllerClass = ABasicEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABasicEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	InitializeComponents();
	
	if (!CombatComponent || !AIController) {
		UE_LOG(LogTopDown2, Error, TEXT("Combat/Health/AI component not found on Enemy"))
		UKismetSystemLibrary::QuitGame(
			this, 
			nullptr,
			EQuitPreference::Quit,
			true // bIgnorePlatformRestrictions
		);
	}

}

void ABasicEnemyCharacter::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);
}

const EEnemyGameState ABasicEnemyCharacter::GetState_Implementation() {
	return CurrentState;
}

UCombatComponent* ABasicEnemyCharacter::GetCombatComponent_Implementation() {
	return CombatComponent;
}

ACharacter* ABasicEnemyCharacter::GetCharacter_Implementation() {
	return this;
}

void ABasicEnemyCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
    AIController = UAIBlueprintHelperLibrary::GetAIController(this);
}

void ABasicEnemyCharacter::SetState_Implementation(const EEnemyGameState NewState) {
	bool bHidden;
	bool bEnableCollision;

	if  (CurrentState != NewState) {
		CombatComponent->ResetHealth();
	}
	
	switch (NewState) {
	case EEnemyGameState::Active:
		bHidden = false;
		bEnableCollision = true;
		if (AIController && AIController->GetBrainComponent()) {
            AIController->GetBrainComponent()->RestartLogic();
		}
		break;
	case EEnemyGameState::Inactive:
		bHidden = true;
		bEnableCollision = false;
		if (AIController && AIController->GetBrainComponent()) {
            AIController->GetBrainComponent()->StopLogic("");
		}
		break;
	default: ;
		bHidden = true;
		bEnableCollision = false;
	}

	CurrentState = NewState;
	SetActorHiddenInGame(bHidden);
	SetActorEnableCollision(bEnableCollision);
}
