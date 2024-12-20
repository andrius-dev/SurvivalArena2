#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameMode.h"
#include "BasePickupItem.generated.h"

UCLASS()
class TOPDOWN2_API ABasePickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickupItem();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	bool bIsPickupActive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	float PitchRotationSpeed = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	float YawRotationSpeed = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	float RollRotationSpeed = 0.f;

	virtual void OnPickedUp();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh = nullptr;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
