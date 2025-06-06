// Fill out your copyright notice in the Description page of Project Settings.


#include "Shottedable.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HelloProjectile.h"
#include "HelloCode/Pawn/HelloCharacter.h"
#include "Kismet/GameplayStatics.h"

UShottedable::UShottedable()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f); // incase of gun
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

		// Spawn the projectile at the muzzle
		World->SpawnActor<AHelloProjectile>(ShotProjectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
	
	// Try and play the sound if specified
	if (ShotSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShotSound, Character->GetActorLocation());
	}
	
	// Get the animation object for the arms mesh
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

	// Set up action bindings
	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the ShotAction when using touch input
			Subsystem->AddMappingContext(ShotMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Triggered, this, &UShottedable::Shot);
		}
	}
}

void UShottedable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Character == nullptr) return;

	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(ShotMappingContext);
		}
	}
}
