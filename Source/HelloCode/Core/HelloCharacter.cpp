// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloCharacter.h"
//#include "HelloShooterProjectile.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "PhysicsEngine/PhysicsAsset.h"

DEFINE_LOG_CATEGORY(LogHelloCharacter);

// Sets default values
AHelloCharacter::AHelloCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bHasWeapon = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	

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
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->AddRelativeRotation(FQuat(FRotator(-5.0f, 0.0f, 0.0f)));
	//CameraComp->FieldOfView = 90.0f;
	//CameraComp->bUsePawnControlRotation = true; // Pawn의 회전값을 사용하도록 설정
	// Create a CameraComponent
	
	// init variables
	MovementSpeed = 10.0f;
	RotationSpeed = 20.0f;

	// init input
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//InputComponent->BindAction("MoveR", IE_Pressed, this, &AHelloCharacter::MoveR);
	//InputComponent->BindAction("MoveL", IE_Pressed, this, &AHelloCharacter::MoveL);

	// init target point
	//m_arrTargetPointPtr.Add(nullptr);

	// init capsule
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHelloCharacter::MyOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHelloCharacter::MyOnComponentEndOverlap);
}

void AHelloCharacter::LookRight(float Value)
{
	if (Value != 0.0f)
	{
		FRotator NewRotation = SpringArmComp->GetRelativeRotation();
		NewRotation.Yaw += Value * RotationSpeed * GetWorld()->GetDeltaSeconds();
		SpringArmComp->SetRelativeRotation(NewRotation); // SpringArm의 회전값을 설정
	}
}

void AHelloCharacter::LookUpper(float Value)
{
	if (Value != 0.0f)
	{
		FRotator NewRotation = SpringArmComp->GetRelativeRotation(); // SpringArm의 회전값을 가져옴
		NewRotation.Pitch += Value * RotationSpeed * GetWorld()->GetDeltaSeconds();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, MinPitchAngle, MaxPitchAngle); // 각도 제한
		SpringArmComp->SetRelativeRotation(NewRotation); // SpringArm의 회전값을 설정
	}
}

void AHelloCharacter::MoveForward(float Value)
{	
	if (Controller && Value != 0.0f)
	{
		// 캐릭터의 정면 방향으로 이동
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHelloCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		// 캐릭터의 오른쪽 방향으로 이동
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
void AHelloCharacter::RotateRight(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value * RotationSpeed * GetWorld()->GetDeltaSeconds());
	}	
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
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHelloCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
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

	// old
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveForward", this, &AHelloCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHelloCharacter::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &AHelloCharacter::RotateRight);	
	PlayerInputComponent->BindAxis("LookRight", this, &AHelloCharacter::LookRight);
	PlayerInputComponent->BindAxis("LookUpper", this, &AHelloCharacter::LookUpper);
	
}

