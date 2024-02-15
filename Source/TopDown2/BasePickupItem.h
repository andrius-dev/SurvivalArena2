// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickupItem.generated.h"

UCLASS()
class TOPDOWN2_API ABasePickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickupItem();
	
	virtual void NotifyHit(
		UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	bool bIsPickupActive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	float PitchRotationSpeed = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	float YawRotationSpeed = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pickup")
	float RollRotationSpeed = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* ItemMesh;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
