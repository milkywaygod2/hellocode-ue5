// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloGameMode.h"
#include "HelloCharacter.h"

AHelloGameMode::AHelloGameMode()
{
	//DefaultPawnClass = AHelloCharacter::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BP_Core/BP_HelloCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AHelloGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 화면 디버거
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 55.0f, FColor::Yellow, FString::Printf(TEXT("======================== : %s"), *FDateTime::Now().ToString()));
	
	// 1 정석
	const FString ClassName1 = FString::Printf(TEXT("%s BeginPlay, until %d!"), *FString("HelloGameModeEcho1"), 2040);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ClassName1);

	// 2 UE_LOG매크로 지원
	const FString ClassName2 = TEXT("HelloGameModeEcho2");
	UE_LOG(LogTemp, Warning, TEXT("%s BeginPlay, until %d!"), *ClassName2, 2040);

	// 3 Format() 가변인자 지원
	const FString ClassName3 = TEXT("HelloGameModeEcho3");
	const FString OtherStringPart = TEXT("BeginPlay, until");
	int32 Year = 2040;
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(ClassName3));
	Args.Add(FStringFormatArg(OtherStringPart));
	Args.Add(FStringFormatArg(Year));
	const FString FormattedString = FString::Format(TEXT("{0} {1} {2}!"), Args); // 타입추론
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FormattedString);


}
