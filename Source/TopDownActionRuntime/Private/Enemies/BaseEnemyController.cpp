#include "BaseEnemyController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter/BasePlayerCharacter.h"
#include "Util/Log.h"

ABaseEnemyController::ABaseEnemyController() {
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemyController::BeginPlay() {
	Super::BeginPlay();
}

void ABaseEnemyController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
}

bool ABaseEnemyController::CanDamagePawn(APawn* InPawn) {
	// for (const auto PawnClass: DamageablePawnTypes) {
	// 	if (InPawn->IsA(PawnClass)) {
	// 		return true;
	// 	}
	// }
	return false;
}
