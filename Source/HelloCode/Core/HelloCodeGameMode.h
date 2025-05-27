// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HelloCodeGameMode.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI) class AHelloCodeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AHelloCodeGameMode();

private:
	UPROPERTY() float GameSpeed;
	UPROPERTY() int32 GameLevel;

public:
	void ScoreUp(unsigned int ScoreUp);
	UFUNCTION() float GetInvGameSpeed() const { return -GameSpeed; }
	UFUNCTION() float GetGameSpeed() const { return GameSpeed; }
	UFUNCTION() int32 GetGameLevel() const { return GameLevel; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 NumCoinsForSpeedIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GameSpeedIncrease;

	virtual void BeginPlay() override;

};
