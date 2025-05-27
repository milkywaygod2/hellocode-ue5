// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UserProfile.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HELLOCODE_API UUserProfile : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	TSubclassOf<UObject> UClassOfPlayer;
	
	UPROPERTY(EditAnywhere, meta=(MetaClass="/Script/Engine.GameMode"), Category = "Unit")
	FSoftClassPath UClassGameMode;
};
