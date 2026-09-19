// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloThing.h"

#include "HelloCode/Able/Pickedable.h"
#include "HelloCode/Able/Shotable.h"


AHelloThing::AHelloThing()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = RootMesh = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("RootMeshComponent")); // TODO: BP매핑 잘되는지 확인
	Pickedable = CreateDefaultSubobject<UPickedable>(TEXT("PickedableComponent"));
	Pickedable->GetPickedableSphere()->SetupAttachment(RootMesh);
}

void AHelloThing::BeginPlay()
{
	Super::BeginPlay();	
	AddDynamicAbilityComponent(UShotable::StaticClass());
	Pickedable->OnPickUp.AddDynamic(TSetAbility.Find(UShotable::StaticClass())->, &UShotable::AttachWeapon);

}

void AHelloThing::AddDynamicAbilityComponent(const TSubclassOf<UActorComponent> AddAbilityClass)
{
	if (!AddAbilityClass) return;
	const TObjectPtr<UActorComponent> NewAbility = NewObject<UActorComponent>(this, AddAbilityClass.Get());
	if (USceneComponent* AttachableAbility = Cast<USceneComponent>(NewAbility))
	{
		AttachableAbility->SetupAttachment(RootMesh);	// 붙이는 위치는 고정
	}
	NewAbility->RegisterComponent();
	TSetAbility.Add(NewAbility);
}

void AHelloThing::RemoveDynamicAbilityComponent(const TSubclassOf<UActorComponent> RemoveAbilityClass)
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

