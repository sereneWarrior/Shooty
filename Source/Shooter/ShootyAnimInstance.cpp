// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

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

	UpdateMovementStatus();
	UpdateCharacterWorldRotation(_DeltaSeconds);
	UpdateAcceleration(_DeltaSeconds);
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

void UShootyAnimInstance::UpdateCharacterWorldRotation(float _DeltaSeconds)
{
	double prev_yaw = CharacterWorldRotation.Yaw;
	CharacterWorldRotation = Owner->GetActorRotation();
	// Current frame yaw rotation difference.
	double delta_yaw = CharacterWorldRotation.Yaw - prev_yaw;
	LeanAngle = FMath::Clamp(UKismetMathLibrary::SafeDivide((float)delta_yaw, _DeltaSeconds),
		-90.0f,90.0f);
	if (MovementDirection == E_Direction::LeftBW ||
		MovementDirection == E_Direction::RightBW ||
		MovementDirection == E_Direction::Backwards)
	{
		LeanAngle *= -1;
	}
}

void UShootyAnimInstance::UpdateMovementStatus()
{
	//Only eveluate walking movement not jumping.
	C_Acceleration2D = FVector(CharacterMovement->GetCurrentAcceleration().X, CharacterMovement->GetCurrentAcceleration().Y, 0.0f);
	// If Acceleration vetor length is close to 0 no movement happens.
	IsAccelerating = !FMath::IsNearlyEqual(C_Acceleration2D.Length(), 0.0f, 0.001f);
}

FVector UShootyAnimInstance::CalculatePhysicalAcceleration(FVector Velocity_delta, float AccelerationValue)
{
	// TODO Rename RelativeAcceleration.
	return Velocity_delta.GetClampedToMaxSize(AccelerationValue) / AccelerationValue;
}

void UShootyAnimInstance::UpdateAcceleration(float _DeltaSeconds)
{

	// Calculate current & previous frame veleocity delta.
	FVector prev_velocity = C_Velocity2D;
	UpdateVelocity();
	FVector delta_velocity = C_Velocity2D - prev_velocity;
	if (_DeltaSeconds == 0.0f)
		return;
	// Return range -1 to 1.
	PhysicalAcceleration2D = IsAccelerating ?
		CalculatePhysicalAcceleration((delta_velocity / _DeltaSeconds), CharacterMovement->MaxAcceleration):
		CalculatePhysicalAcceleration((delta_velocity / _DeltaSeconds), CharacterMovement->GetMaxBrakingDeceleration());
}
	