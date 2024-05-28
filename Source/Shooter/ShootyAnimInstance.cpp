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

	UpdateRotationAndProcessDelta(_DeltaSeconds);
	UpdateVelocity(_DeltaSeconds);
	UpdateMovementStatus();
}

void UShootyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	
}

void UShootyAnimInstance::CalculateMovementDirection(float locomationAngle, float FMin,
	float FMax, float BMin, float BMax)
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

void UShootyAnimInstance::UpdateRotationAndProcessDelta(float _DeltaSeconds)
{
	/*
	* Update World Rotation,
	* Calculate delta betrween previous and current world rotation to get the angle for Leaning animation.
	*/
	// TODO: Seperate funcuionalities by creating property to store previous rotation?
	double prev_yaw = CharacterWorldRotation.Yaw;
	CharacterWorldRotation = Owner->GetActorRotation();
	double delta_yaw = CharacterWorldRotation.Yaw - prev_yaw;
	if (!FMath::IsNearlyEqual(delta_yaw, 0.0f, 0.001f))
		CalculateLeanAngle(delta_yaw, _DeltaSeconds);
}

void UShootyAnimInstance::CalculateLeanAngle(double delta_yaw, float _DeltaSeconds)
{
	/*
	* Set LeanAngle for leaning additive animation Blendspace
	*/
	LeanAngle = FMath::Clamp(UKismetMathLibrary::SafeDivide((float)delta_yaw, _DeltaSeconds),
		-90.0f, 90.0f);
	if (MovementDirection == E_Direction::LeftBW ||
		MovementDirection == E_Direction::RightBW ||
		MovementDirection == E_Direction::Backwards)
	{
		LeanAngle *= -1;
	}
}


void UShootyAnimInstance::UpdateMovementStatus()
{
	/*
	* Evaluates Acceleration a 2D vector. Therefore only conciders ground movement (not jumping).
	* If Acceleration vetor length is close to 0 no movement happens.
	*/
	FVector acceleration2D = FVector(CharacterMovement->GetCurrentAcceleration().X, CharacterMovement->GetCurrentAcceleration().Y, 0.0f);
	bIsAccelerating = !FMath::IsNearlyEqual(acceleration2D.Length(), 0.0f, 0.001f);
}

void UShootyAnimInstance::UpdateVelocity(float _DeltaSeconds)
{
	/*
	* Update Velocity.
	* Calculate delta between previous and curretn velocity to drive acceleration based leaning Blendspace.
	*/
	// TODO: Seperate funcuionalities by creating property to store previous velocity?
	FVector prev_velocity = Velocity2D;
	Velocity2D = FVector(CharacterMovement->Velocity.X, CharacterMovement->Velocity.Y, 0.0f);
	FVector delta_velocity = Velocity2D - prev_velocity;
	SetRelativeAcceleration(delta_velocity, _DeltaSeconds);
}

void UShootyAnimInstance::SetRelativeAcceleration(FVector delta_velocity, float _DeltaSeconds)
{
	/*
	* Set Acceleration as delta between current and previous velocity.
	* Value needs clamped and be mapped to values ranging from -1 to 1. (Blendspace requirement.)
	*/
	if (_DeltaSeconds == 0.0f)
		return;
	RelativeAcceleration2D = bIsAccelerating ?
		ClampAcceleration((delta_velocity / _DeltaSeconds), CharacterMovement->MaxAcceleration):
		ClampAcceleration((delta_velocity / _DeltaSeconds), CharacterMovement->GetMaxBrakingDeceleration());
}

FVector UShootyAnimInstance::ClampAcceleration(FVector Velocity_delta, float AccelerationValue)
{
	return Velocity_delta.GetClampedToMaxSize(AccelerationValue) / AccelerationValue;
}

	