// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HelloCode/Pawn/HelloCharacter.h"
#include "Pickable.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AHelloCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELLOCODE_API UPickable : public UPrimitiveComponent
{
	GENERATED_BODY()
	
	UPrimitiveComponent* PickupCollision;

	UPickable();

	UPROPERTY(BlueprintAssignable, Category = "Interaction") FOnPickUp OnPickUp;
public:	
	void SetPickupMesh(UPrimitiveComponent* InputMesh);
	UPrimitiveComponent* GetPickupMesh() const { return PickupCollision; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnPrimitiveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
