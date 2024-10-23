#include "EnemySpawnFlow.h"

UEnemySpawnFlowContinuous::UEnemySpawnFlowContinuous() {
	MaxCurrentCount = ENEMY_COUNT_MAX;
	SecsBetweenSpawns = 0;
}

int UEnemySpawnFlowContinuous::GetMaxCurrentCount() {
	return MaxCurrentCount;
}

double UEnemySpawnFlowContinuous::GetSecsBetweenSpawns() {
	return SecsBetweenSpawns;
}

EEnemySpawnFlowType UEnemySpawnFlowContinuous::GetModeType() {
	if (MaxCurrentCount < 0) {
		return EEnemySpawnFlowType::ContinuousUnlimited;
	}
	return EEnemySpawnFlowType::ContinuousUnlimited; // todo maybe remove lim/unlim
}

UEnemySpawnModeWaves::UEnemySpawnModeWaves() {
}

EEnemySpawnFlowType UEnemySpawnModeWaves::GetModeType() {
	return EEnemySpawnFlowType::Waves;
}
