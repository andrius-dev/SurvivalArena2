#include "BaseEnemyController.h"

#include "TopDown2/PlayerCharacter/TopDown2Character.h"
#include "TopDown2/Util/Log.h"

// Sets default values
ABaseEnemyController::ABaseEnemyController() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemyController::BeginPlay() {
	Super::BeginPlay();
}

void ABaseEnemyController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	if (BaseBehaviourTree == nullptr) {
		LogPawnAction("Behaviour tree not found!", InPawn->GetName());
		return;
	}
}

bool ABaseEnemyController::IsKickableAss(APawn* InPawn) {
	LogPawnAction("Looking for ass kicking", InPawn->GetName());
	for (const auto Ass : KickableAssTypes)
	{
		if (InPawn->IsA(Ass))
		{
			LogPawnAction("This ass is kickable: ", InPawn->GetName());	
			return true;		
		}
	}	
	LogPawnAction("This ass can't be kicked", InPawn->GetName());	
	return false;
}

// todo make basic functions for logging: for errors, warnings, etc.
void ABaseEnemyController::LogPawnAction(const FString& PawnAction, const FString& Name) {
	UE_LOG(
		LogTopDown2,
		Log,
		TEXT("%s: %s"),
		*Name,
		*PawnAction
	);
}
