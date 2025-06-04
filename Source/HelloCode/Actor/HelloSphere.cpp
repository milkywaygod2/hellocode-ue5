// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloSphere.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AHelloSphere::AHelloSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트-구체컴포넌트
	SphereCompo = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereCompo;
	SphereCompo->SetSphereRadius(220.0f);
	SphereCompo->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// StaticMesh & Material 추가
	SphereStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereStaticMesh->SetupAttachment(RootComponent);	
	ConstructorHelpers::FObjectFinder<UStaticMesh> assetSphereShape(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	ConstructorHelpers::FObjectFinder<UMaterial> assetSphereMaterial(TEXT("Material'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'"));
	if (assetSphereShape.Succeeded() && assetSphereMaterial.Succeeded())
	{		
		SphereStaticMesh->SetStaticMesh(assetSphereShape.Object);
		SphereStaticMesh->SetMaterial(0, assetSphereMaterial.Object);		
		SphereStaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load StaticMesh!"));
	}

	// 파티클 추가
	SphereParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	SphereParticle->SetupAttachment(SphereStaticMesh);
	SphereParticle->SetAutoActivate(true);

	ConstructorHelpers::FObjectFinder<UParticleSystem> assetFireParticle(TEXT("ParticleSystem'/Engine/Tutorial/SubEditors/TutorialAssets/TutorialParticleSystem.TutorialParticleSystem'"));
	if (assetFireParticle.Succeeded())
	{
		SphereParticle->SetTemplate(assetFireParticle.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load ParticleSystem!"));
	}

	// 3D Text 추가
	SphereTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	SphereTextRender->SetupAttachment(SphereStaticMesh);
	SphereTextRender->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SphereTextRender->SetHorizontalAlignment((EHTA_Center));
	SphereTextRender->SetYScale(2.0f);
	SphereTextRender->SetXScale(2.0f);
	SphereTextRender->SetText(NSLOCTEXT("GreetingNs", "HelloKey", "HelloWorld"));

	// 델리게이트 Event 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AHelloSphere::MyOnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHelloSphere::MyOnEndOverlap);
}

void AHelloSphere::MyOnBeginOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	const FString OutputString = "Hello " + OtherActor->GetName();
	SphereTextRender->SetText(FText::FromString(OutputString));
}

void AHelloSphere::MyOnEndOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	SphereTextRender->SetText(NSLOCTEXT("GreetingNs", "HelloKey", "HelloWorld"));
}



