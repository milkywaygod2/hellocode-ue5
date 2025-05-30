// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloCharacter.h"
//#include "HelloShooterProjectile.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ArrowComponent.h"
#include "Engine/LocalPlayer.h"

#include "PhysicsEngine/PhysicsAsset.h"

DEFINE_LOG_CATEGORY(LogHelloCharacter);

void AHelloCharacter::UseForwardArrow()
{
	UArrowComponent* ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrow->SetupAttachment(RootComponent);
	ForwardArrow->ArrowSize = 1.0f;
	ForwardArrow->SetRelativeLocation(FVector(50.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	ForwardArrow->bHiddenInGame = false; // 인게임에서 표시
}

// Sets default values
AHelloCharacter::AHelloCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// init variables
	bHasWeapon = false;

	
	UseForwardArrow();
		
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(CameraComp);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// SkeletalMesh
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> AssetSkeletalMesh(TEXT("SkeletalMesh'/Game/FBX/jellyfish.jellyfish'"));
	const ConstructorHelpers::FObjectFinder<UPhysicsAsset> AssetPhysics(TEXT("PhysicsAsset'/Game/FBX/jellyfish_PhysicsAsset.jellyfish_PhysicsAsset'"));
	
	if (AssetSkeletalMesh.Succeeded() && AssetPhysics.Succeeded())
	{		
		GetMesh()->SetSkeletalMesh(AssetSkeletalMesh.Object);
		GetMesh()->SetPhysicsAsset(AssetPhysics.Object); // TODO: SetPhysicsAsset 적용..
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SkeletalMesh!"));
	}

	// setting character
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;

	// init camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	check(SpringArmComp);
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 10.0f;
	SpringArmComp->AddRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->AddRelativeRotation(FQuat(FRotator(-5.0f, 0.0f, 0.0f)));
	//CameraComp->FieldOfView = 90.0f;

	// init input
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// init capsule
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHelloCharacter::MyOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHelloCharacter::MyOnComponentEndOverlap);
}

void AHelloCharacter::MyOnComponentOverlap(UPrimitiveComponent* pOverlappedComponent, AActor* pOtherActor,
	UPrimitiveComponent* pOtherComp, int32 nOtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
}

void AHelloCharacter::MyOnComponentEndOverlap(UPrimitiveComponent* pOverlappedComponent, AActor* pOtherActor,
	UPrimitiveComponent* pOtherComp, int32 nOtherBodyIndex)
{
}

void AHelloCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// 프레임속도에 따른 보정 + 대각선 속도보정
		const float DeltaTime = GetWorld()->GetDeltaSeconds();
		const float DiagonalFactor = (FMath::Abs(MovementVector.X) > 0 && FMath::Abs(MovementVector.Y) > 0) ? 0.707f : 1.0f;
		AddMovementInput(GetActorForwardVector(), MovementVector.Y * DeltaTime * InputKeyboardMoveSpeed * DiagonalFactor);
		AddMovementInput(GetActorRightVector(), MovementVector.X * DeltaTime * InputKeyboardMoveSpeed * DiagonalFactor);
	}
}

void AHelloCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// 프레임속도에 따른 보정
		const float DeltaTime = GetWorld()->GetDeltaSeconds();        
		AddControllerYawInput(LookAxisVector.X * DeltaTime * InputMouseTurnSpeed);
		AddControllerPitchInput(LookAxisVector.Y * DeltaTime * InputMouseTurnSpeed);
	}
}

// Called when the game starts or when spawned
void AHelloCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	UAnimSequence* AssetAnim = LoadObject<UAnimSequence>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/FBX/jellyfish_Anim.jellyfish_Anim'"));
	if (AssetAnim)
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->PlayAnimation(AssetAnim, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Anim!"));
	}	
}

// Called every frame
void AHelloCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHelloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(InputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHelloCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHelloCharacter::Look);
	}
	else
	{
		UE_LOG(LogHelloCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}	
}

