// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

// TODO: Is this needed?
void FBaseAnimInstanceProxy::SetVelocityData()
{
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("FBaseAnimInstanceProxy::SetVelocityData");
	if (!CharacterMovement)
		return;
	//OwnerAnimInstance->C_Velocity = CharacterMovement->Velocity;
	//OwnerAnimInstance->C_Velocity2D = FVector(OwnerAnimInstance->C_Velocity.X, OwnerAnimInstance->C_Velocity.Y, 0.0f);
}

void FBaseAnimInstanceProxy::Initialize(UAnimInstance* AnimInstance)
{
	Super::Initialize(AnimInstance);
	if (!AnimInstance) return;
	
	OwnerAnimInstance = Cast<UShootyAnimInstance>(AnimInstance);
	// TODO: DO I need this cast? This should do:
	// Cast<ACharacterMovementComponent>(OwnerAnimInstance->TryGetPawnOwner()->GetMovementComponent());
	OwnerAnimInstance->Owner = Cast<AShooty>(Owner);
	if (Owner)
	{
		CharacterMovement = Owner->GetCharacterMovement();
		//Owner = Cast<AShooty>(Owner);
	}
}

void FBaseAnimInstanceProxy::Update(float DeltaSeconds)
{
	SetVelocityData();
}


void UShootyAnimInstance::NativeBeginPlay()
{
	Owner = Cast<AShooty>(GetOwningActor());
	if (Owner)
	{
		CharacterMovement = Owner->GetCharacterMovement();
	}
}

void UShootyAnimInstance::NativeThreadSafeUpdateAnimation(float _DeltaSeconds)
{
	if (!CharacterMovement)
		return;

	UpdateVelocity();
	UpdateCharacterWorldRotation();
	UpdateMovementStatus();
}

void UShootyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	
}

void UShootyAnimInstance::CalculateMovementDirection(float locomationAngle, float FMin, float FMax, float BMin, float BMax)
{
	//TODO: Should I check that angle is in the right range?
	// Backwards:
	if (locomationAngle <= BMin || locomationAngle >= BMax)
		MovementDirection = E_MovementDirection::Backwards;
	// Forwards:
	else if (locomationAngle >= FMin && locomationAngle <= FMax)
		MovementDirection = E_MovementDirection::Forewards;
	// Left:
	else if (locomationAngle <= FMin && locomationAngle >= BMin)
		MovementDirection = E_MovementDirection::Left;
	//Right:
	else
		MovementDirection = E_MovementDirection::Right;
}

void UShootyAnimInstance::UpdateVelocity()
{
	C_Velocity2D = FVector(CharacterMovement->Velocity.X, CharacterMovement->Velocity.Y, 0.0f);
}

void UShootyAnimInstance::UpdateCharacterWorldRotation()
{
	CharacterWorldRotation = Owner->GetActorRotation();
}

void UShootyAnimInstance::UpdateMovementStatus()
{
	//Only eveluate walking movementnot jumping.
	C_Acceleration2D = FVector(CharacterMovement->GetCurrentAcceleration().X, CharacterMovement->GetCurrentAcceleration().Y, 0.0f);
	// If Acceleration vetor length is close to 0 no movement happens.
	IsMoving = !FMath::IsNearlyEqual(C_Acceleration2D.Length(), 0.0f, 0.001f);
}
