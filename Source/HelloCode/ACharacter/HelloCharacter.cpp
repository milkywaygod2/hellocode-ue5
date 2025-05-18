// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloCharacter.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AHelloCharacter::AHelloCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SkeletalMesh
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> AssetSkeletalMesh(TEXT("SkeletalMesh'/Game/FBX/jellyfish.jellyfish'"));
	if (AssetSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(AssetSkeletalMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SkeletalMesh!"));
	}

	// setting character
	GetCapsuleComponent()->InitCapsuleSize(42.f, 24.f);
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

	// init variables
	CurrentLocation = 0;
	DesiredLocation = FVector(0.0f, 0.0f, 0.0f);
	bPushed = false;
	GameScore = 0;
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

void AHelloCharacter::MoveToTargetR()
{
	if (Controller != nullptr)
	{
		if (CurrentLocation < ArrTargetPointPtr.Num() - 1/*toIndex0*/)
		{
			++CurrentLocation;
		}
		else
		{
			// nothing	to do
		}
	}
}

void AHelloCharacter::MoveToTargetL()
{
	if (Controller != nullptr)
	{
		if (CurrentLocation > 0)
		{
			--CurrentLocation;
		}
		else
		{
			// nothing	to do
		}
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

// Called when the game starts or when spawned
void AHelloCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 레핑-상태연동기능추가(동적변경등 특수한 상황아니면 에디터 생성후 연결권장)
	//ConstructorHelpers::FObjectFinder<UAnimBlueprint> AssetAnim(TEXT("AnimBlueprint'/Game/HelloCode/Character/Animation/HelloAnimBP.HelloAnimBP'"));
	UAnimSequence* AssetAnim = LoadObject<UAnimSequence>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/FBX/jellyfish_Anim.jellyfish_Anim'"));
	if (AssetAnim)
	{
		//GetMesh()->SetAnimInstance(AssetAnim.Object->GeneratedClass);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->PlayAnimation(AssetAnim, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Anim!"));
	}
	
	// TODO: target point
	// for (TActorIterator<ATargetPoint> itActorPtr(GetWorld()); itActorPtr; ++itActorPtr)
	// {
	// 	ATargetPoint* pTargetPoint = *itActorPtr;
	// 	if (pTargetPoint)
	// 	{
	// 		ArrTargetPointPtr.Add(pTargetPoint);
	// 		UE_LOG(LogTemp, Warning, TEXT("Found TargetPoint: %s"), *pTargetPoint->GetName());
	// 	}
	// }
	// auto predLocation = [](const AActor& A, const AActor& B) ->bool { return A.GetActorLocation().Y < B.GetActorLocation().Y; };
	// ArrTargetPointPtr.Sort(predLocation);
	// CurrentLocation = ((ArrTargetPointPtr.Num() / 2) + (ArrTargetPointPtr.Num() % 2) - 1/*toIndex0*/);
}

void AHelloCharacter::ScoreUp()
{
	
}

// Called every frame
void AHelloCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ArrTargetPointPtr.Num() > 0)
	{
		FVector vTargetLocation = ArrTargetPointPtr[CurrentLocation]->GetActorLocation();
		vTargetLocation.Z = GetActorLocation().Z;
		vTargetLocation.X = GetActorLocation().X;
		if (vTargetLocation != GetActorLocation())
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), vTargetLocation, MovementSpeed * DeltaTime)); // Lerp: linear interpolation
		}
	}
	else
	{
		// nothing to do
	}
}

// Called to bind functionality to input
void AHelloCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(InputComponent);
	InputComponent->BindAction("MoveToTargetR", IE_Pressed, this, &AHelloCharacter::MoveToTargetR);
	InputComponent->BindAction("MoveToTargetL", IE_Pressed, this, &AHelloCharacter::MoveToTargetL);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveForward", this, &AHelloCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHelloCharacter::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &AHelloCharacter::RotateRight);	
	PlayerInputComponent->BindAxis("LookRight", this, &AHelloCharacter::LookRight);
	PlayerInputComponent->BindAxis("LookUpper", this, &AHelloCharacter::LookUpper);
	
}

