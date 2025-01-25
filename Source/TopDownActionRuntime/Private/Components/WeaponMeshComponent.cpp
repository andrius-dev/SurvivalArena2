#include "Components/WeaponMeshComponent.h"

UWeaponMeshComponent::UWeaponMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponMeshComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
