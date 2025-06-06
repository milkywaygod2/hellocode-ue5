// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shotable.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOCODE_API UShotable : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	/** Can be nullptr, some Shotable-thing could be mounted out of sight like a missle in the cell */	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shot", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshCollision;
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AHelloProjectile> ShotProjectile;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* ShotSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ShotAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* ShotMappingContext;

	/** Shot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ShotAction;

	/** Sets default values for this component's properties */
	UShotable();

	/** Make the weapon Shot a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Shot();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AHelloCharacter* TargetCharacter);

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AHelloCharacter* Character;
};
