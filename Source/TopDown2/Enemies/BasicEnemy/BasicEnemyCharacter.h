#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BasicEnemyCharacter.generated.h"

UCLASS()
class TOPDOWN2_API ABasicEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicEnemyCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(Category=AI, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<UClass*> KickableAssTypes;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AI|Components|PawnSensing")
	void MoveToAss(AActor* Target);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AI|Components|PawnSensing")
	bool IsKickableAss(APawn* Pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void LogPawnAction(const FString& PawnAction, const FString& Name);
	UPROPERTY()
	AAIController *SelfAIController;
	
};
