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
	// TODO: 주운 순간 Shotable 의 AttachWeapon 을 물리는 배선이 아직 없다.
	//       아래 줄은 TSetAbility.Find(...) 가 돌려준 것에서 UShotable* 를 꺼내는 자리가
	//       비어 있어(`->` 뒤가 없다) 컴파일되지 않는다. 꺼내는 길을 정한 뒤 되살린다.
	//Pickedable->OnPickUp.AddDynamic(TSetAbility.Find(UShotable::StaticClass())->, &UShotable::AttachWeapon);

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

