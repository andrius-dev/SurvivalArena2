#include "TopDown2/Enemies/BasicEnemy/BaseEnemyCharacter.h"
#include "BaseEnemyController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDown2/Util/Log.h"

ABaseEnemyCharacter::ABaseEnemyCharacter() {
	CurrentState = EEnemyGameState::Inactive;
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	AIControllerClass = ABaseEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CombatAttributeSet = CreateDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}

void ABaseEnemyCharacter::BeginPlay() {
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

void ABaseEnemyCharacter::InitAttributes() {
}

void ABaseEnemyCharacter::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);
}

const EEnemyGameState ABaseEnemyCharacter::GetState_Implementation() {
	return CurrentState;
}

UCombatComponent* ABaseEnemyCharacter::GetCombatComponent_Implementation() {
	return CombatComponent;
}

ACharacter* ABaseEnemyCharacter::GetCharacter_Implementation() {
	return this;
}

AActor* ABaseEnemyCharacter::GetTargetActor_Implementation() {
	return TargetActor;
}

void ABaseEnemyCharacter::SetTargetActor_Implementation(AActor* NewTargetActor) {
	TargetActor = NewTargetActor;
}

void ABaseEnemyCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	InitAttributes();
}

UAbilitySystemComponent* ABaseEnemyCharacter::GetAbilitySystemComponent() const {
	return AbilitySystem;
}

void ABaseEnemyCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
    AIController = UAIBlueprintHelperLibrary::GetAIController(this);
}

void ABaseEnemyCharacter::EventSpawned_Implementation(AActor* Spawner) {
	// todo: need
	IEnemyCharacterInterface::EventSpawned_Implementation(Spawner);
}

void ABaseEnemyCharacter::SetState_Implementation(const EEnemyGameState NewState) {
	bool bHidden;
	bool bEnableCollision;

	if  (IsValid(CombatAttributeSet) && CurrentState != NewState) {
		CombatAttributeSet->ResetHealth();
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
