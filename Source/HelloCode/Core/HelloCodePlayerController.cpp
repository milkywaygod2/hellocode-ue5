// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloCodePlayerController.h"
#include "EnhancedInputSubsystems.h"

void AHelloCodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
}
