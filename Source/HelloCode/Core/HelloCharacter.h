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
	// mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;
	
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

	float InputMouseTurnSpeed{10.f};
	float InputKeyboardMoveSpeed{30.f};

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

	USkeletalMeshComponent* GetCharacterMesh() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComp; }
	
	UPROPERTY(EditAnywhere, Category = Logic) float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = Logic) float RotationSpeed;

	UFUNCTION() void MyOnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION() void MyOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// sound
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* HitAudioComp;
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* CoinAudioComp;

public:


	

};
