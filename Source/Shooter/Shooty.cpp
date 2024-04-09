// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooty.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "ShootyAnimInstance.h"


// Sets default values
AShooty::AShooty(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UExtendedMovementComponent>(CharacterMovementComponentName))
{
	ExtendedCharacterMovement = Cast<UExtendedMovementComponent>(GetCharacterMovement());
	if (ExtendedCharacterMovement)
	{
		ExtendedCharacterMovement->UpdatedComponent =GetCapsuleComponent();
	}
		


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Character movement
	// TODO: Toggle when moving.
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Body = GetMesh();

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	//CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//FollowCamera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void AShooty::BeginPlay()
{
	Super::BeginPlay();
	// Set up input mapping context.
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{			
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	// Limit Camera Pitch( up/down) rotation.
	//TODO: Make value BP editable.
	auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CameraManager->ViewPitchMax = ViewPitchMax;
	CameraManager->ViewPitchMin = ViewPitchMin;

	if(ExtendedCharacterMovement == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("SAMAE"));

	// Set Gait to Walking. 
	//UpdateGait(EGaitTEST::Walking);
}

// Called every frame
void AShooty::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooty::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooty::Look);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooty::Move);
		
		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Started, this, &AShooty::StartJog);
		// TODO: Are there cases where I not return into Walking?
		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Completed, this, &AShooty::StopJog);
	}

}

void AShooty::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShooty::Move(const FInputActionValue& Value)
{
	
	FVector2D MoveAxisVector = Value.Get<FVector2D>();
	
	// Change walk speed when moving backwards.
	//if (MoveAxisVector.Y < 0)
	//{
	//	//NO!
	//	ExtendedCharacterMovement->BackwardsPressed();
	//}
	//ExtendedCharacterMovement->BackwardsReleased();
	//Setting Movement mode instead of using Map.
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Swimming);

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// Calculate World direction (forward & right) & set movement
		AddMovementInput(UKismetMathLibrary::GetRightVector(Rotation), MoveAxisVector.X);
		AddMovementInput(UKismetMathLibrary::GetForwardVector(Rotation), MoveAxisVector.Y);
	}
}

void AShooty::StartJog(const FInputActionValue& Value)
{
	ExtendedCharacterMovement->JogPressed();
	UpdateGait(EGait::Jogging);
}

void AShooty::StopJog(const FInputActionValue& Value)
{
	ExtendedCharacterMovement->JogReleased();
	UpdateGait(EGait::Walking);
}

void  AShooty::UpdateGait(const EGait newGait)
{
	
	// Update CurrentGait in AnimInstance
	// TODO: Do I need the interface here?
	
	if (UShootyAnimInstance* animInstance = Cast<UShootyAnimInstance>(Body->GetAnimInstance()))
	{
		animInstance->ReceiveGaitStatus(newGait);
	}
	
}



