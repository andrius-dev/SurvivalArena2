#pragma once
#include "EnemySpawnFlow.generated.h"

UENUM(BlueprintType)
enum class EEnemySpawnFlowType : uint8 {
	None,
	ContinuousLimited,
	ContinuousUnlimited,
	Waves
};

UCLASS(BlueprintType)
class TOPDOWN2_API UEnemySpawnFlow : public UObject {
	GENERATED_BODY()

public:
	virtual EEnemySpawnFlowType GetModeType() {
		return EEnemySpawnFlowType::None;
	}
};

UCLASS()
class TOPDOWN2_API UEnemySpawnFlowContinuous final : public UEnemySpawnFlow {
	GENERATED_BODY()
public:
	UEnemySpawnFlowContinuous();
	
	UFUNCTION(BlueprintCallable, Category="GameState")
	int GetMaxCurrentCount();
	
	UFUNCTION(BlueprintCallable, Category="GameState")
	double GetSecsBetweenSpawns();
	
	virtual EEnemySpawnFlowType GetModeType() override;
	static constexpr int ENEMY_COUNT_MAX = 50;
	
private:
	int MaxCurrentCount;
	double SecsBetweenSpawns;
};


UCLASS()
class TOPDOWN2_API UEnemySpawnModeWaves : public UEnemySpawnFlow {
	GENERATED_BODY()
public:
	UEnemySpawnModeWaves();
	virtual EEnemySpawnFlowType GetModeType() override;
private:
	// todo 
};
