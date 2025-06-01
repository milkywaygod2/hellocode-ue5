// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HelloCharacter.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogHelloCharacter, Log, All);

UCLASS(Config=Game)
class HELLOCODE_API AHelloCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AHelloCharacter();

private:
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	// input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	bool bHasWeapon;

	// param
	float InputMouseTurnSpeed;
	float InputKeyboardMoveSpeed;
	float InputSpringArmPitchMin;
	float InputSpringArmPitchMax;
	float InputSpringArmYawMin;
	float InputSpringArmYawMax;
	float ControllerTurnSpeed;

	void InitArrow();
	void InitMesh();
	void InitCamera();

protected:
	// move
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// default
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = Weapon) void SetHasWeapon(const bool bNewHasWeapon) { bHasWeapon = bNewHasWeapon; }
	UFUNCTION(BlueprintCallable, Category = Weapon) bool GetHasWeapon() const { return bHasWeapon; }

	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComp; }
	
	UFUNCTION() void MyOnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION() void MyOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// sound
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* HitAudioComp;
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* CoinAudioComp;

public:


	

};
