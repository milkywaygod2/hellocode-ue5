// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickedable.h"

UPickedable::UPickedable() : PickedableSphereRadius(1.0f)
{
	PrimaryComponentTick.bCanEverTick = true;

	PickedableSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DefaultPickedableSphere"));
	PickedableSphere->SetHiddenInGame(true);	
	PickedableSphere->SetSphereRadius(PickedableSphereRadius); // 컴포넌트가 완전히 초기화된 후 반영
	PickedableSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickedableSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UPickedable::PostInitProperties()
{
	Super::PostInitProperties();    
}

void UPickedable::BeginPlay()
{
	Super::BeginPlay();
	PickedableSphere->OnComponentBeginOverlap.AddDynamic(this, &UPickedable::OnSphereBeginOverlap);
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
		PickedableSphere->OnComponentBeginOverlap.RemoveAll(this);
	}
}

