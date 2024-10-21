#include "EnemySpawnMode.h"

UEnemySpawnModeContinuous::UEnemySpawnModeContinuous() {
	MaxCurrentCount = ENEMY_COUNT_MAX;
	SecsBetweenSpawns = 0;
}

EEnemySpawnType UEnemySpawnModeContinuous::GetModeType() {
	if (MaxCurrentCount < 0) {
		return EEnemySpawnType::ContinuousUnlimited;
	}
	return EEnemySpawnType::ContinuousUnlimited; // todo maybe remove lim/unlim
}

UEnemySpawnModeWaves::UEnemySpawnModeWaves() {
}

EEnemySpawnType UEnemySpawnModeWaves::GetModeType() {
	return EEnemySpawnType::Waves;
}
