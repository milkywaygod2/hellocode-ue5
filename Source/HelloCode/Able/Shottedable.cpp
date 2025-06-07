// Fill out your copyright notice in the Description page of Project Settings.


#include "Shottedable.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HelloCode/Actor/HelloProjectile.h"
#include "HelloCode/Pawn/HelloCharacter.h"
#include "Kismet/GameplayStatics.h"

UShottedable::UShottedable()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f); // incaseof gun
}

void UShottedable::Shot()
{
	if (Character == nullptr || Character->GetController() == nullptr) return;
	
	if (UWorld* const World = GetWorld(); World && ShotProjectile)
	{
		const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// 1.액터생성
		World->SpawnActor<AHelloProjectile>(ShotProjectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
	
	// 2.사운드 재생
	if (ShotSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShotSound, Character->GetActorLocation());
	}
	
	// 3.애니메이션 재생
	if (UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance(); AnimInstance && ShotAnimation)
	{
		AnimInstance->Montage_Play(ShotAnimation, 1.f);
	}
}

void UShottedable::AttachWeapon(AHelloCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr || Character->GetEquipShottedable()) return;
	
	// Attach the weapon to the First Person Character
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	Character->SetEquipShottedable(true);

	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		// 1. 인풋 키매핑 컨텍스트 추가
		if (UEnhancedInputLocalPlayerSubsystem* PlayerEnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			PlayerEnhancedInputSubsystem->AddMappingContext(ShotMappingContext, 1); // 키 매핑의 누적적 추가, 매핑중복시 0 보다 더 높은 우선순위
		}

		// 2. 인풋 콜백함수 바인딩
		if (UEnhancedInputComponent* PlayerEnhancedInput = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			PlayerEnhancedInput->BindAction(ShotAction, ETriggerEvent::Triggered, this, &UShottedable::Shot);
		}
	}
}

void UShottedable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Character == nullptr) return;

	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		// 3. 인풋 키매핑 컨텍스트 제거
		if (UEnhancedInputLocalPlayerSubsystem* PlayerEnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			PlayerEnhancedInputSubsystem->RemoveMappingContext(ShotMappingContext);
		}
	}
}
