#include "BasicEnemyCharacter.h"

#include <string>

#include "Perception/PawnSensingComponent.h"
#include "TopDown2/PlayerCharacter/TopDown2Character.h"

// Sets default values
ABasicEnemyCharacter::ABasicEnemyCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasicEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	SelfAIController = Cast<AAIController>(Controller);
}


// Called every frame
void ABasicEnemyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasicEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


bool ABasicEnemyCharacter::IsKickableAss(APawn* Pawn) {
	LogPawnAction("Looking for ass kicking", Pawn->GetName());
	for (auto Ass : KickableAssTypes)
	{
		if (Pawn->IsA(Ass))
		{
			LogPawnAction("This ass is kickable: ", Pawn->GetName());	
			return true;		
		}
	}	
	LogPawnAction("This ass can't be kicked", Pawn->GetName());	
	return false;
}

void ABasicEnemyCharacter::MoveToAss(AActor* Target) {
	LogPawnAction("Moving to kick ass", Target->GetName());
	SelfAIController->MoveToActor(Target);
}

void ABasicEnemyCharacter::LogPawnAction(const FString& PawnAction, const FString& Name) {
	UE_LOG(
		LogTemplateCharacter,
		Log,
		TEXT("%s: %s"),
		*Name,
		*PawnAction
	);
}
