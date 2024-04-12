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
		MovementDirection = E_Direction::Backwards;
	// Forwards:
	else if (locomationAngle >= FMin && locomationAngle <= FMax)
		MovementDirection = E_Direction::Forewards;
	// Left:
	else if (locomationAngle <= FMin && locomationAngle >= BMin)
	{
		if (MovementDirection == E_Direction::Right
			|| MovementDirection == E_Direction::LeftBW
			|| MovementDirection == E_Direction::Backwards)
		{
			MovementDirection = E_Direction::LeftBW;
			return;
		}
		MovementDirection = E_Direction::Left;
	}
	//Right:
	else
	{
		if (MovementDirection == E_Direction::Left ||
			MovementDirection == E_Direction::RightBW ||
			MovementDirection == E_Direction::Backwards)
		{
			MovementDirection = E_Direction::RightBW;
			return;
		}
		MovementDirection = E_Direction::Right;
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
