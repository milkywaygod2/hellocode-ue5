// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HelloCode/Pawn/HelloCharacter.h"
#include "Pickedable.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AHelloCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELLOCODE_API UPickedable : public USphereComponent
{
	GENERATED_BODY()
public:
	UPickedable();

private:
	float PickedableSphereRadius;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:		
	UPROPERTY(BlueprintAssignable, Category = "Interaction") FOnPickUp OnPickUp;

	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetPickedableSphereRadius() const { return PickedableSphereRadius; }
	void SetPickedableSphereRadius(const float Radius) { this->PickedableSphereRadius = Radius; }
public:
};
