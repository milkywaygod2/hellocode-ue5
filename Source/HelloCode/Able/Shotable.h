// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shotable.generated.h"

class AHelloCharacter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOCODE_API UShotable : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UShotable();

private:
	AHelloCharacter* AbledCharacter;

public:
	AHelloCharacter* GetAbledCharacter() const { return AbledCharacter; }

	void SetAbledCharacter(AHelloCharacter* AbledCharacter) { this->AbledCharacter = AbledCharacter; }

protected:
	UFUNCTION() virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AHelloProjectile> ShotProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* ShotSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ShotAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* ShotMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ShotAction;

	UFUNCTION(BlueprintCallable, Category="Shot")
	void Shot();

	UFUNCTION(BlueprintCallable, Category="Shot")
	void AttachWeapon(AHelloCharacter* TargetCharacter);
};
