// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickupItem.h"

// Sets default values
ABasePickupItem::ABasePickupItem() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh item"));
	RootComponent = ItemMesh;
}

void ABasePickupItem::NotifyHit(
	UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit
) {
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	Destroy();
}

// Called when the game starts or when spawned
void ABasePickupItem::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ABasePickupItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	FRotator deltaRotation(
		PitchRotationSpeed * DeltaTime,
		YawRotationSpeed * DeltaTime,
		RollRotationSpeed * DeltaTime
	);
	RootComponent->AddWorldRotation(deltaRotation);
}
