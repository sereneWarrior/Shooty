// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UShootyAnimInstance::ReceiveGaitStatus(EGait gaitStatus)
{
	CurrentGait = gaitStatus;
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
	//TODO: Should I check that angle is in the right range?<=180/>=-180
	// Backwards:
	if (locomationAngle <= BMin || locomationAngle >= BMax)
	{
		HipFacingDirection = E_HipFacingDirection::Backwards;
		MovementDirection = E_MovementDirection::Backwards;
	}
	// Forwards:
	else if (locomationAngle >= FMin && locomationAngle <= FMax)
	{
		HipFacingDirection = E_HipFacingDirection::Forewards;
		MovementDirection = E_MovementDirection::Forewards;
	}
	// Left:
	else if (locomationAngle <= FMin && locomationAngle >= BMin)
	{
		CalculateHipFacingDirection(MovementDirection, E_MovementDirection::Left);
		MovementDirection = E_MovementDirection::Left;
	}
	//Right:
	else
	{
		CalculateHipFacingDirection(MovementDirection, E_MovementDirection::Right);
		MovementDirection = E_MovementDirection::Right;
	}
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


void UShootyAnimInstance::CalculateHipFacingDirection(E_MovementDirection oldDirection, E_MovementDirection newDirection) {
	// If previous MovementDirection was F or B just take the same value

	if (oldDirection == newDirection)
		return;

	switch (oldDirection)
	{
	case E_MovementDirection::Forewards:
		HipFacingDirection = E_HipFacingDirection::Forewards;
		break;
	case E_MovementDirection::Backwards:
		HipFacingDirection = E_HipFacingDirection::Backwards;
		break;
	case E_MovementDirection::Left:
	case E_MovementDirection::Right:
		// TODO: Create ReverseHipFacingDirection method.
		if (HipFacingDirection == E_HipFacingDirection::Forewards)
			HipFacingDirection = E_HipFacingDirection::Backwards;
		else
			HipFacingDirection = E_HipFacingDirection::Forewards;
		break;
	}

}