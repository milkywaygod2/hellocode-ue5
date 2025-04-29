// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HelloCharacter.generated.h"

UCLASS()
class HELLOCODE_API AHelloCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AHelloCharacter();
	
private:	
	// game-logic
	bool bPushed;
	short CurrentLocation;
	FVector DesiredLocation;

	// Dev
	bool bTestCharacterMesh = false;
	
protected:
	// score
	UPROPERTY(BlueprintReadOnly) int32 GameScore;
	
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera) float MinPitchAngle = -45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera) float MaxPitchAngle = 45.0f;
	void LookRight(float Value);
	void LookUpper(float Value);
	
	// move
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);
	UPROPERTY(EditAnywhere, Category = Logic) TArray<class ATargetPoint*> ArrTargetPointPtr;
	UPROPERTY(EditAnywhere, Category = Logic) float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = Logic) float RotationSpeed;
	void MoveToTargetR();
	void MoveToTargetL();

	//UFUNCTION() void MyOnComponentOverlap(UPrimitiveComponent* pOverlappedComponent, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, FVector vNormalImpulse, const FHitResult& Hit);
	UFUNCTION() void MyOnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION() void MyOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// sound
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* HitAudioComp;
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* CoinAudioComp;

	void ScoreUp();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
