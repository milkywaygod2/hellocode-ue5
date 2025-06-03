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

void AHelloCharacter::InitArrow()
{
	UArrowComponent* ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrow->SetupAttachment(RootComponent);
	ForwardArrow->ArrowSize = 2.0f;
	ForwardArrow->SetRelativeLocation(FVector(250.0f, 0.0f, 0.0f));
	ForwardArrow->bHiddenInGame = false; // 인게임에서 표시
}

void AHelloCharacter::InitMesh()
{
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> AssetSkeletalMesh(TEXT("SkeletalMesh'/Game/FBX/jellyfish.jellyfish'"));
	const ConstructorHelpers::FObjectFinder<UPhysicsAsset> AssetPhysics(TEXT("PhysicsAsset'/Game/FBX/jellyfish_PhysicsAsset.jellyfish_PhysicsAsset'"));
	
	if (AssetSkeletalMesh.Succeeded() && AssetPhysics.Succeeded())
	{		
		GetMesh()->SetSkeletalMesh(AssetSkeletalMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		
		GetMesh()->SetPhysicsAsset(AssetPhysics.Object); // TODO: SetPhysicsAsset 적용..

		FVector MeshBounds = AssetSkeletalMesh.Object->GetBounds().BoxExtent;    
		float CapsuleRadius = MeshBounds.Y;  // 너비(Y) 기준으로 반지름 설정
		float CapsuleHalfHeight = MeshBounds.Z;  // 높이(Z) 기준으로 설정
		GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHelloCharacter::MyOnComponentOverlap);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHelloCharacter::MyOnComponentEndOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SkeletalMesh!"));
	}
}

void AHelloCharacter::InitCamera()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	check(SpringArmComp);
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = -SpringArmLength; // -앞으로 뻗기, +뒤로 뻗기
	SpringArmComp->AddRelativeLocation(FVector(0.f, 0.0f, 100.0f)); // 앞뒤, 좌우, 상하
	SpringArmComp->bUsePawnControlRotation = false; // 카메라가 캐릭터의 회전을 따르지 않도록 설정
	
	UStaticMeshComponent* SpringArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpringArmMesh"));
	SpringArmMesh->SetupAttachment(SpringArmComp);
	SpringArmMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"))); // 기본 메시 지정
	SpringArmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 방지
	SpringArmMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 1.0f)); // 스프링암 길이 맞추기
	SpringArmMesh->CastShadow = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->AddRelativeRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	//CameraComp->FieldOfView = 90.0f;
	//DrawDebugLine(GetWorld(), SpringArmComp->GetComponentLocation(), CameraComp->GetComponentLocation(), FColor::Blue, true, -1, 0, 2.0f);
}

// Sets default values
AHelloCharacter::AHelloCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// init param
	bHasWeapon = false;
	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;
	InputMouseTurnSpeed = 50.0f; // 마우스 회전 속도
	InputKeyboardMoveSpeed = 30.0f; // 키보드 이동 속도
	InputCameraPitchMin = -75.0f;
	InputCameraPitchMax = 75.0f; 
	InputCameraYawMinMax = 55.0f;
	SpringArmLength = 50.0f; // 스프링암 길이
	InputSpringArmPitchMin = -45.0f;
	InputSpringArmPitchMax = 30.0f;
	InputSpringArmYawMinMax =  30.0f;
	SpringArmTurnSpeed = 20.0f;
	ControllerTurnSpeed = 10.0f;
	bIsFixedNeck = false;
	
	// init component
	InitArrow();
	InitMesh();
	InitCamera();
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
	if (Controller != nullptr)
	{
		const float DeltaTime(GetWorld()->GetDeltaSeconds());
		const FRotator OldControllerRot(GetControlRotation());
		const FRotator OldRelativeSpringArmRot(SpringArmComp->GetRelativeRotation());

		const FRotator ControllerAdjustedRot(FRotator(
			FMath::Clamp(OldRelativeSpringArmRot.Pitch * DeltaTime, -ControllerTurnSpeed, ControllerTurnSpeed),
			FMath::Clamp(OldRelativeSpringArmRot.Yaw * DeltaTime, -ControllerTurnSpeed, ControllerTurnSpeed),
			FMath::Clamp(OldRelativeSpringArmRot.Roll * DeltaTime, -ControllerTurnSpeed, ControllerTurnSpeed)
		));
		Controller->SetControlRotation(FRotator(
			OldControllerRot.Pitch + ControllerAdjustedRot.Pitch,
			OldControllerRot.Yaw + ControllerAdjustedRot.Yaw,
			OldControllerRot.Roll + ControllerAdjustedRot.Roll
		));
		SpringArmComp->SetRelativeRotation(FRotator(
			OldRelativeSpringArmRot.Pitch - ControllerAdjustedRot.Pitch,
			OldRelativeSpringArmRot.Yaw - ControllerAdjustedRot.Yaw,
			OldRelativeSpringArmRot.Roll - ControllerAdjustedRot.Roll
		));
		
		const FVector2D MovementVector = Value.Get<FVector2D>();
		const float DiagonalFactor = (FMath::Abs(MovementVector.X) > 0 && FMath::Abs(MovementVector.Y) > 0) ? 0.707f : 1.0f;
		AddMovementInput(GetActorForwardVector(), MovementVector.Y * DeltaTime * InputKeyboardMoveSpeed * DiagonalFactor);
		AddMovementInput(GetActorRightVector(), MovementVector.X * DeltaTime * InputKeyboardMoveSpeed * DiagonalFactor);
	}
}

void AHelloCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const float DeltaTime(GetWorld()->GetDeltaSeconds());
		const FVector2D LookAxisVector = Value.Get<FVector2D>();
		const FRotator OldRelativeCameraRot(CameraComp->GetRelativeRotation());
		const FRotator NewRelativeCameraRot = FRotator(
			 FMath::Clamp(OldRelativeCameraRot.Pitch + LookAxisVector.Y * DeltaTime * InputMouseTurnSpeed,
				InputCameraPitchMin,
				InputCameraPitchMax),
			FMath::Clamp(OldRelativeCameraRot.Yaw + LookAxisVector.X * DeltaTime * InputMouseTurnSpeed,
				-InputCameraYawMinMax,
				InputCameraYawMinMax), 0.0f
		);
		CameraComp->SetRelativeRotation(NewRelativeCameraRot);
		
		//TODO: 머리최대로 돌리면 자동회전
		//TODO: 목 피치를 항시 약간 들고 있기
		//TODO: 경계 속도 부드럽게
	}
}

void AHelloCharacter::TickNeck(const bool Value)
{
	if (!Value)
	{
		if (Controller)
		{
			const float DeltaTime(GetWorld()->GetDeltaSeconds());
			const FRotator OldControllerRot(GetControlRotation());
			const FRotator OldRelativeSpringArmRot(SpringArmComp->GetRelativeRotation());
			const FRotator OldRelativeCameraRot(CameraComp->GetRelativeRotation());

			const FRotator SpringArmAdjustedRot(FRotator(
				FMath::Clamp(OldRelativeCameraRot.Pitch * DeltaTime, -SpringArmTurnSpeed, SpringArmTurnSpeed),
				FMath::Clamp(OldRelativeCameraRot.Yaw * DeltaTime, -SpringArmTurnSpeed, SpringArmTurnSpeed),
				0.0f
			));

			float NewSpringArmPitch, NewCameraPitch;
			if (OldRelativeSpringArmRot.Pitch + SpringArmAdjustedRot.Pitch > InputSpringArmPitchMax)
			{
				NewSpringArmPitch = InputSpringArmPitchMax;
				NewCameraPitch = OldRelativeCameraRot.Pitch - (NewSpringArmPitch - OldRelativeSpringArmRot.Pitch);
			}
			else if (OldRelativeSpringArmRot.Pitch + SpringArmAdjustedRot.Pitch < InputSpringArmPitchMin)
			{
				NewSpringArmPitch = InputSpringArmPitchMin;
				NewCameraPitch = OldRelativeCameraRot.Pitch - (NewSpringArmPitch - OldRelativeSpringArmRot.Pitch);
			}
			else
			{
				NewSpringArmPitch = OldRelativeSpringArmRot.Pitch + SpringArmAdjustedRot.Pitch;
				NewCameraPitch = OldRelativeCameraRot.Pitch - SpringArmAdjustedRot.Pitch;
			}

			float NewSpringArmYaw, NewCameraYaw;
			if (OldRelativeSpringArmRot.Yaw + SpringArmAdjustedRot.Yaw > InputSpringArmYawMinMax)
			{
				NewSpringArmYaw = InputSpringArmYawMinMax;
				NewCameraYaw = OldRelativeCameraRot.Yaw - (NewSpringArmYaw - OldRelativeSpringArmRot.Yaw);
			}
			else if (OldRelativeSpringArmRot.Yaw + SpringArmAdjustedRot.Yaw < -InputSpringArmYawMinMax)
			{
				NewSpringArmYaw = -InputSpringArmYawMinMax;
				NewCameraYaw = OldRelativeCameraRot.Yaw - (NewSpringArmYaw - OldRelativeSpringArmRot.Yaw);
			}
			else
			{
				NewSpringArmYaw = OldRelativeSpringArmRot.Yaw + SpringArmAdjustedRot.Yaw;
				NewCameraYaw = OldRelativeCameraRot.Yaw - SpringArmAdjustedRot.Yaw;
			}

			SpringArmComp->SetRelativeRotation(FRotator(NewSpringArmPitch, NewSpringArmYaw, 0.0f));
			CameraComp->SetRelativeRotation(FRotator(NewCameraPitch, NewCameraYaw, 0.0f));
		}
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
	TickNeck(bIsFixedNeck);
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
		EnhancedInputComponent->BindAction(FixNeckAction, ETriggerEvent::Started, this, &AHelloCharacter::FixNeck);
		EnhancedInputComponent->BindAction(FixNeckAction, ETriggerEvent::Completed, this, &AHelloCharacter::UnFixNeck);
	}
	else
	{
		UE_LOG(LogHelloCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}	
}

