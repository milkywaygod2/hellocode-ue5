// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AHelloProjectile::AHelloProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	ProjectileCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	ProjectileCollisionComp->InitSphereRadius(5.0f);
	ProjectileCollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileCollisionComp->OnComponentHit.AddDynamic(this, &AHelloProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	ProjectileCollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	ProjectileCollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = ProjectileCollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComp->UpdatedComponent = ProjectileCollisionComp;
	ProjectileMovementComp->InitialSpeed = 3000.f;
	ProjectileMovementComp->MaxSpeed = 3000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AHelloProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

// Called when the game starts or when spawned
void AHelloProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHelloProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

