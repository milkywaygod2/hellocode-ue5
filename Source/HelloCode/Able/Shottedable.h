// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shottedable.generated.h"

class AHelloCharacter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOCODE_API UShottedable : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UShottedable();

private:
	AHelloCharacter* Character;
	
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
