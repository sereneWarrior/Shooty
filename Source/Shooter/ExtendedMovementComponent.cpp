// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedMovementComponent.h"
#include "GameFramework/Character.h"


UExtendedMovementComponent::UExtendedMovementComponent()
{
}

void UExtendedMovementComponent::SetGaitState(EGait movementStatus)
{
	// TODO: Do I have to consider movement mode here
	
	if (FGaitSetting* setting = GaitSettings.Find(movementStatus))
	{
		MaxForewardsWalkSpeed = setting->MaxWalkSpeed;
		MaxBackwardsWalkSpeed = setting->MaxBackwardsWalkSpeed;
		MaxAcceleration = setting->MaxAcceleration;
		BrakingDecelerationWalking = setting->BrakingDeceleration;
		BrakingFriction = setting->BrakingFriction;
		bUseSeparateBrakingFriction = setting->UseSeperateBreakingFriction;
		BrakingFrictionFactor = setting->BrakingFrictionFactor;
	}
	
	
}

void UExtendedMovementComponent::JogPressed()
{
	//bIsJogging = true;
}

void UExtendedMovementComponent::JogReleased()
{
	//bIsJogging = false;
}

void UExtendedMovementComponent::IsMovingBackwards(bool isMovingBackwards)
{
	bIsMovingBackwards = isMovingBackwards;
}

void UExtendedMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	if (MovementMode != MOVE_Walking)
		return;

	// TODO: Is called every frame. Better if Speed is only set when Movementstatus changes.
	MaxWalkSpeed = bIsMovingBackwards ? MaxBackwardsWalkSpeed : MaxForewardsWalkSpeed;

}
