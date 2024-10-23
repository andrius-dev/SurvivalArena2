#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "BasicEnemyController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDown2/Util/Log.h"

ABasicEnemyCharacter::ABasicEnemyCharacter() {
	CurrentState = EEnemyGameState::Inactive;
}

void ABasicEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	AIControllerClass = ABasicEnemyController::StaticClass();
	CombatComponent = FindComponentByClass<UCombatComponent>();
	HealthComponent = FindComponentByClass<UHealthComponent>();
	
	if (!CombatComponent || !HealthComponent) {
		UE_LOG(LogTopDown2, Error, TEXT("Combat/Health component not found on Enemy"))
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

// todo: merge combat and health component!!
UCombatComponent* ABasicEnemyCharacter::GetCombatComponent_Implementation() {
	return CombatComponent;
}

ACharacter* ABasicEnemyCharacter::GetCharacter_Implementation() {
	return this;
}

UHealthComponent* ABasicEnemyCharacter::GetHealthComponent_Implementation() {
	return HealthComponent;
}

void ABasicEnemyCharacter::SetStateCpp(EEnemyGameState State) {
	SetState(State);
}

const EEnemyGameState ABasicEnemyCharacter::GetStateCpp() {
	return GetState();
}

ACharacter* ABasicEnemyCharacter::GetCharacterCpp() {
	return GetCharacter();
}

UHealthComponent* ABasicEnemyCharacter::GetHealthComponentCpp() {
	return GetHealthComponent();
}

UCombatComponent* ABasicEnemyCharacter::GetCombatComponentCpp() {
	return GetCombatComponent();
}

void ABasicEnemyCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	CachedPawn = NewController->GetPawn();
}

void ABasicEnemyCharacter::SetState_Implementation(const EEnemyGameState NewState) {
	if (!CachedPawn) {
		return;
	}
	CurrentState = NewState;
	bool bHidden;
	bool bEnableCollision;
	
	switch (CurrentState) {
	case EEnemyGameState::Active:
		bHidden = false;
		bEnableCollision = true;
		Cast<AAIController>(GetController())->GetBrainComponent()->RestartLogic();
		break;
	case EEnemyGameState::Inactive:
		bHidden = true;
		bEnableCollision = false;
		Cast<AAIController>(GetController())->GetBrainComponent()->StopLogic("");
		break;
	default: ;
		bHidden = true;
		bEnableCollision = false;
	}

	SetActorHiddenInGame(bHidden);
	SetActorEnableCollision(bEnableCollision);
}
