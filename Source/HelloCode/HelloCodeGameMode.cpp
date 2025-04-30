// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloCodeGameMode.h"
#include "ACharacter/HelloCharacter.h"

AHelloCodeGameMode::AHelloCodeGameMode()
{
	DefaultPawnClass = AHelloCharacter::StaticClass();

	NumCoinsForSpeedIncrease = 5;
	GameSpeed = 10.0f;
	GameSpeedIncrease = 5.0f;
	GameLevel = 1;
}

void AHelloCodeGameMode::ScoreUp(unsigned int ScoreUp)
{
	if (ScoreUp != 0 && ((ScoreUp % NumCoinsForSpeedIncrease) == 0))
	{
		GameSpeed += GameSpeedIncrease;
		GameLevel++;
	}
}
