// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedMovementComponent.h"
#include "GameFramework/Character.h"

UExtendedMovementComponent::UExtendedMovementComponent()
{
}

void UExtendedMovementComponent::JogPressed()
{
	bIsJogging = true;
}

void UExtendedMovementComponent::JogReleased()
{
	bIsJogging = false;
}

void UExtendedMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	if (MovementMode != MOVE_Walking)
		return;

	if (Velocity.X <0)
	{
		MaxWalkSpeed = bIsJogging ? MaxBackwardsSpeed_Jog : MaxBackwardsSpeed_Walk;
		return;
	}
	
	MaxWalkSpeed = bIsJogging ? MaxSpeed_Jog : MaxSpeed_Walk;
	
}
