// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownGameMode.h"

int ATopDownGameMode::GetScore() const {
	return score;
}

void ATopDownGameMode::SetScore(const int Score) {
	score = Score;
}

std::string ATopDownGameMode::GetFormattedScore() const {
	return std::to_string(score);
}
