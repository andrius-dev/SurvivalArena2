#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "BasicEnemyController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDown2/Util/Log.h"

ABasicEnemyCharacter::ABasicEnemyCharacter() {
	CurrentState = EEnemyGameState::Inactive;
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AIControllerClass = ABasicEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABasicEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	InitializeComponents();
	
	if (!CombatComponent || !HealthComponent || !AIController) {
		UE_LOG(LogTopDown2, Error, TEXT("Combat/Health/AI component not found on Enemy"))
		UKismetSystemLibrary::QuitGame(
			this, 
			nullptr,
			EQuitPreference::Quit,
			true // bIgnorePlatformRestrictions
		);
	}

}

void ABasicEnemyCharacter::DispatchOnEnemyDefeated(UHealthComponent* EnemyHealthComponent) {
	if (OnDefeatedListener) {
		IEnemyDefeatedListenerInterface::Execute_OnEnemyDefeated(OnDefeatedListener.GetObject(), this);
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

void ABasicEnemyCharacter
::BindOnDefeatedEvent_Implementation(UObject* Listener) {
	UE_LOG(LogTopDown2, All, TEXT("Binding enemy defeated event"))
	if (Listener->GetClass()->ImplementsInterface(UEnemyDefeatedListenerInterface::StaticClass())) {
		OnDefeatedListener = Listener;
		HealthComponent->OnDeath.AddUniqueDynamic(this, &ABasicEnemyCharacter::DispatchOnEnemyDefeated);
		UE_LOG(LogTopDown2, All, TEXT("Bound enemy defeated event"))
	} else {
		UE_LOG(LogTopDown2, All, TEXT("Failed to bind enemy defeated event"))
	}
}

void ABasicEnemyCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
    AIController = UAIBlueprintHelperLibrary::GetAIController(this);
}

void ABasicEnemyCharacter::SetState_Implementation(const EEnemyGameState NewState) {
	CurrentState = NewState;
	bool bHidden;
	bool bEnableCollision;
	
	switch (CurrentState) {
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

	SetActorHiddenInGame(bHidden);
	SetActorEnableCollision(bEnableCollision);
}
