// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "HelloSphere.generated.h"

//UCLASS(Blueprintable)
UCLASS(BlueprintType)
class HELLOCODE_API AHelloSphere : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AHelloSphere();

private:
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void MyOnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);	
	void MyOnBeginOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION(BlueprintNativeEvent)
	void MyOnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);	
	void MyOnEndOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(Category="Component", EditAnywhere)
	class USphereComponent* SphereCompo;
	
	UPROPERTY(Category="Component", EditAnywhere)
	class UStaticMeshComponent* SphereStaticMesh;

	UPROPERTY(Category="Component", EditAnywhere, BlueprintReadOnly)
	class UTextRenderComponent* SphereTextRender;

	UPROPERTY(Category="Component", EditAnywhere)
	class UParticleSystemComponent* SphereParticle;	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override { Super::BeginPlay(); }

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override { Super::Tick(DeltaTime); }
};
