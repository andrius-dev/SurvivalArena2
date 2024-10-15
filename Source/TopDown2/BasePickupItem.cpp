#include "BasePickupItem.h"

ABasePickupItem::ABasePickupItem() {
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh item"));
	RootComponent = ItemMesh;
}

void ABasePickupItem::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
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
