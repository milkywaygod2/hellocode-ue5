// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AHelloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
}
