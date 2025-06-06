// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloThing.h"


// Sets default values
AHelloThing::AHelloThing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHelloThing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHelloThing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

