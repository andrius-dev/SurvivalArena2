#include "TopDown2/Enemies/BasicEnemy/BasicEnemyCharacter.h"
#include "BasicEnemyController.h"

// Sets default values
ABasicEnemyCharacter::ABasicEnemyCharacter()
{
}

// Called when the game starts or when spawned
void ABasicEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AIControllerClass = ABasicEnemyController::StaticClass();
	
	if (DefaultBehaviorTree == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("DefaultBehaviorTree is null"));
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
