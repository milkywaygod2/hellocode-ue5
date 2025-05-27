// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HelloCodePlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class HELLOCODE_API AHelloCodePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
	virtual void BeginPlay() override;
	// End Actor interface
};
