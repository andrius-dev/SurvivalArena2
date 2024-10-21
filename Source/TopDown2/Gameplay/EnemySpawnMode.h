#pragma once
#include "EnemySpawnMode.generated.h"

UENUM(BlueprintType)
enum class EEnemySpawnType : uint8 {
	None,
	ContinuousLimited,
	ContinuousUnlimited,
	Waves
};

UCLASS(BlueprintType)
class TOPDOWN2_API UEnemySpawnMode : public UObject {
	GENERATED_BODY()

public:
	virtual EEnemySpawnType GetModeType() {
		return EEnemySpawnType::None;
	}
};


UCLASS()
class TOPDOWN2_API UEnemySpawnModeContinuous final : public UEnemySpawnMode {
	GENERATED_BODY()
public:
	UEnemySpawnModeContinuous();
	virtual EEnemySpawnType GetModeType() override;
	static constexpr int ENEMY_COUNT_MAX = 50;
	
private:
	int MaxCurrentCount;
	double SecsBetweenSpawns;
};


UCLASS()
class TOPDOWN2_API UEnemySpawnModeWaves : public UEnemySpawnMode {
	GENERATED_BODY()
public:
	UEnemySpawnModeWaves();
	virtual EEnemySpawnType GetModeType() override;
private:
	// todo 
};
