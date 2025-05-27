// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloCodeGameMode.h"
#include "HelloCharacter.h"

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

void AHelloCodeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 1 정석
	const FString ClassName1 = FString::Printf(TEXT("%s BeginPlay, until %d!"), *FString("HelloCodeGameMode"), 2040);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ClassName1);

	// 2 UE_LOG매크로 지원
	const FString ClassName2 = TEXT("HelloCodeGameMode");
	UE_LOG(LogTemp, Warning, TEXT("%s BeginPlay, until %d!"), *ClassName2, 2040);

	// 3 Format() 가변인자 지원
	const FString ClassName3 = TEXT("HelloCodeGameMode");
	const FString OtherStringPart = TEXT("BeginPlay, until");
	int32 Year = 2040;
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(ClassName3));
	Args.Add(FStringFormatArg(OtherStringPart));
	Args.Add(FStringFormatArg(Year));
	const FString FormattedString = FString::Format(TEXT("{0} {1} {2}!"), Args); // 타입추론
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FormattedString);
	
}
