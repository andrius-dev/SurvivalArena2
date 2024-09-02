#include "BasicEnemySensing.h"

#include "AIController.h"
#include "TopDown2/PlayerCharacter/TopDown2PlayerController.h"

void UBasicEnemySensing::BeginPlay() {
	Super::BeginPlay();
}

void UBasicEnemySensing::BroadcastOnSeePawn(APawn& Pawn) {
	Super::BroadcastOnSeePawn(Pawn);
	SelfAIController = CastChecked<AAIController, AController>(GetSensorController());
}

bool UBasicEnemySensing::IsKickableAss(const APawn& Pawn) {
	LogPawnAction("Looking for ass kicking", Pawn.GetName());
	return KickableAssTypes.Find(Pawn.GetClass()) != INDEX_NONE;
}

void UBasicEnemySensing::MoveToAss(AActor* Target) {
	LogPawnAction("Moving to kick ass", Target->GetName());
	SelfAIController->MoveToActor(Target);
}

void UBasicEnemySensing::LogPawnAction(const FString& PawnAction, const FString& Name) {
	UE_LOG(
		LogTemplateCharacter,
		Log,
		TEXT("&s: &s"),
		*Name,
		*PawnAction
	);
}
