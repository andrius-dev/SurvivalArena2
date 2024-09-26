#pragma once

#include "CoreMinimal.h"
#include "WeaponMeshComponent.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN2_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	// todo should be the same as collider
	const float DEFAULT_HIT_RADIUS = 20.f;
	const float DEFAULT_DAMAGE = 5.f;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void DetectMeleeHits();
	
	virtual FString GetReadableName() const override {
		return "CombatComponent";
	}

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	TObjectPtr<UWeaponMeshComponent> EquippedWeaponMesh = nullptr;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore = TArray<AActor*>();

	UPROPERTY()
	float DamagePerHit = DEFAULT_DAMAGE;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Combat")
	float HitRadius = DEFAULT_HIT_RADIUS;

private:	
	UPROPERTY()
	FVector BladeStart = FVector();

	UPROPERTY()
	FVector BladeEnd = FVector();
};
