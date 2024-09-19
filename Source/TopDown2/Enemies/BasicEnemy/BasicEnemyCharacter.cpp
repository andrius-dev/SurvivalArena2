// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"

#include "AIController.h"
#include "BasicEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationCommon.h"

// Sets default values
ABasicEnemyCharacter::ABasicEnemyCharacter()
{
}

// Called when the game starts or when spawned
void ABasicEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultBehaviorTree == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("DefaultBehaviorTree is null"));
		return;
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&ABasicEnemyCharacter::RunAIBehavior,
		4.0,
		false
	);
}

void ABasicEnemyCharacter::RunAIBehavior() {
	// auto World = GetWorld();
	// if (World == nullptr) {
	// 	UE_LOG(LogTemp, Error, TEXT("World is null"));
	// 	return;
	// }
	// auto EnemyController = Cast<ABasicEnemyController>(Controller);
	// auto Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	
	// EnemyController->RunAIBehaviors(DefaultBehaviorTree, World, Character);	
}

// Called every frame
void ABasicEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
