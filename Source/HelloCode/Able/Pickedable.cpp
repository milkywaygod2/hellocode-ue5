// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickedable.h"

UPickedable::UPickedable() : Super(), PickedableSphereRadius(0.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetHiddenInGame(true);
	SetSphereRadius(PickedableSphereRadius);
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UPickedable::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UPickedable::OnSphereBeginOverlap);
	//OnPickUp.AddDynamic(GetOwner<AHelloCharacter>(), &AHelloCharacter::HandlePickUp); // 현재는 브로드케스이후 실행되는것이 없음
}

void UPickedable::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPickedable::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AHelloCharacter* Character = Cast<AHelloCharacter>(OtherActor))
	{
		OnPickUp.Broadcast(Character);
		OnComponentBeginOverlap.RemoveAll(this);
	}
}

