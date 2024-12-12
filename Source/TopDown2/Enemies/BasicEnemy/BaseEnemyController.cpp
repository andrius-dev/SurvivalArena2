#include "BaseEnemyController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "TopDown2/PlayerCharacter/BasePlayerCharacter.h"
#include "TopDown2/Util/Log.h"

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
	for (const auto Pawn : DamageablePawnTypes) {
		if (InPawn->IsA(Pawn)) {
			return true;
		}
	}
	return false;
}
