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
	SpringArmComp->TargetArmLength = -50.0f; // -앞으로 뻗기, +뒤로 뻗기
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

	// init component
	InitArrow();
	InitMesh();
	InitCamera();

	// init param
	bHasWeapon = false;
	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;
	InputMouseTurnSpeed = 50.0f; // 마우스 회전 속도
	InputKeyboardMoveSpeed = 30.0f; // 키보드 이동 속도
	InputSpringArmPitchMin = -75.0;
	InputSpringArmPitchMax = 75.0; 
	InputSpringArmYawMin = -45.0;  
	InputSpringArmYawMax = 45.0;
	ControllerTurnSpeed = 10.0f;
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
		const FRotator NewRelativeSpringArmRot = FRotator(
			 FMath::Clamp(SpringArmComp->GetRelativeRotation().Pitch + LookAxisVector.Y * DeltaTime * InputMouseTurnSpeed,
				InputSpringArmPitchMin,
				InputSpringArmPitchMax),
			FMath::Clamp(SpringArmComp->GetRelativeRotation().Yaw + LookAxisVector.X * DeltaTime * InputMouseTurnSpeed,
				InputSpringArmYawMin,
				InputSpringArmYawMax),0.0f
		);
		SpringArmComp->SetRelativeRotation(NewRelativeSpringArmRot);
		
		//TODO: 천천히 움직일때는 카메라(머리), 빨리움직일떄는 스프링암(목) -> 카메라순으로
		//TODO: 머리 목 회전 최대제한
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

