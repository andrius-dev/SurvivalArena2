#include "TopDown2/Components/AimArrowComponent.h"

#include "Kismet/KismetMathLibrary.h"

void UAimArrowComponent::AimToTarget(const FVector& Target) {
	const auto LookRotation =
		UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), Target);
	SetWorldRotation(LookRotation);
}
