#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponMeshComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWN2_API UWeaponMeshComponent : public UStaticMeshComponent {
	GENERATED_BODY()

public:
	UWeaponMeshComponent();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

protected:
	virtual void BeginPlay() override;
};
