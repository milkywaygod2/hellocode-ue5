// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickedable.h"

#include "Components/SphereComponent.h"

UPickedable::UPickedable()
{
	PrimaryComponentTick.bCanEverTick = true;
	PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DefaultPickupSphere"));
}

UPickedable::UPickedable(UPrimitiveComponent* InputMesh)
{
	SetPickupMesh(InputMesh);
}

void UPickedable::SetPickupMesh(UPrimitiveComponent* InputMesh)
{
	PickupCollision = InputMesh ? InputMesh : CreateDefaultSubobject<USphereComponent>(TEXT("DefaultPickupSphere"));

	PickupCollision->SetGenerateOverlapEvents(true);
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts
void UPickedable::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UPickedable::OnPrimitiveBeginOverlap); // UPrimitiveComponent 이벤트 델리게이트
	//OnPickUp.AddDynamic(GetOwner<AHelloCharacter>(), &AHelloCharacter::HandlePickUp); // 현재는 브로드케스이후 실행되는것이 없음
}


// Called every frame
void UPickedable::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPickedable::OnPrimitiveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AHelloCharacter* Character = Cast<AHelloCharacter>(OtherActor); Character != nullptr)
	{
		OnPickUp.Broadcast(Character);
		OnComponentBeginOverlap.RemoveAll(this);
	}
}

