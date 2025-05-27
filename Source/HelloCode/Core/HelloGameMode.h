// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HelloGameMode.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI) class AHelloGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AHelloGameMode();

private:

protected:
	virtual void BeginPlay() override;

public:
};
