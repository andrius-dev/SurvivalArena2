#include "BasicEnemyCharacter.h"

#include "TopDown2/PlayerCharacter/TopDown2Character.h"

// Sets default values
ABasicEnemyCharacter::ABasicEnemyCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasicEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
}


// Called every frame
void ABasicEnemyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasicEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
