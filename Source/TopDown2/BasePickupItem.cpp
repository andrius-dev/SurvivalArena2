// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickupItem.h"

#include "GameTags.h"

// Sets default values
ABasePickupItem::ABasePickupItem() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh item"));
	RootComponent = ItemMesh;
	GameMode = CreateDefaultSubobject<AGameMode>(TEXT("Game mode"));
}

void ABasePickupItem::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	// todo check for component instead of tag
	if (OtherActor->ActorHasTag(FName(GameTags::Player))) {
		OnPickedUp();
		Destroy();
	}
}

void ABasePickupItem::OnPickedUp() {
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
