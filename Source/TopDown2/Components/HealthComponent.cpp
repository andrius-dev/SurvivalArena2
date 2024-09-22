#include "TopDown2/Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;
}

UHealthComponent* UHealthComponent::FindHealthComponent(const AActor* Actor) {
	if (!Actor) {
		return nullptr;
	}
	return Actor->FindComponentByClass<UHealthComponent>();
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UHealthComponent::GetMaxHealth() const {
	return MaxHealth;
}

float UHealthComponent::GetCurrentHealth() {
	return CurrentHealth;
}

float UHealthComponent::TakeDamage(const float Amount) {
	const float OldHealth = CurrentHealth;
	CurrentHealth -= Amount;
	if (CurrentHealth < 0.f) {
		CurrentHealth = 0.f;
	}
	
	OnHealthChanged.Broadcast(
		this,
		OldHealth,
		CurrentHealth,
		this->GetOwner()
	);
	
	if (CurrentHealth == 0) {
		OnDeath.Broadcast(this);
	}

	// there will be calculations later
	return Amount;
}
