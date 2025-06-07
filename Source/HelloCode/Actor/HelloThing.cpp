// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloThing.h"

#include "HelloCode/Able/Pickedable.h"
#include "HelloCode/Able/Shottedable.h"


AHelloThing::AHelloThing()
{
	PrimaryActorTick.bCanEverTick = true;

	RootMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RootMeshComponent"));
	RootComponent = RootMeshComponent;

	Pickedable = CreateDefaultSubobject<UPickedable>(TEXT("PickedableComponent"));
	Pickedable->SetupAttachment(RootComponent);
}

void AHelloThing::BeginPlay()
{
	Super::BeginPlay();	
	//AddDynamicAbilityComponent(UShottedable::StaticClass());
}

void AHelloThing::AddDynamicAbilityComponent(const TSubclassOf<UPrimitiveComponent> AddAbilityClass)
{
	if (!AddAbilityClass) return;
	const TObjectPtr<UPrimitiveComponent> NewAbility = NewObject<UPrimitiveComponent>(this, AddAbilityClass.Get());
	NewAbility->SetupAttachment(Pickedable);	// 붙이는 윔치는 고정
	NewAbility->RegisterComponent();
	TSetAbility.Add(NewAbility);
}

void AHelloThing::RemoveDynamicAbilityComponent(const TSubclassOf<UPrimitiveComponent> RemoveAbilityClass)
{
	if (!RemoveAbilityClass) return;

	for (const TObjectPtr Ability : TSetAbility)
	{
		if (Ability && Ability->GetClass() == RemoveAbilityClass.Get())
		{
			TSetAbility.Remove(Ability);
			if (IsValid(Ability)) Ability->DestroyComponent();
			break;
		}
	}
}

void AHelloThing::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

