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

void UExtendedMovementComponent::IsMovingBackwards(bool movesBackwards)
{
	bMovesBackwards = movesBackwards;
}

void UExtendedMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	if (MovementMode != MOVE_Walking)
		return;

	// TODO: Is called every frame. Better if Speed is only set when Movementstatus changes.
	if (bMovesBackwards)
	{
		MaxWalkSpeed = bIsJogging ? MaxBackwardsSpeed_Jog : MaxBackwardsSpeed_Walk;
		return;
	}
	
	// TODO:Move into pressed/ released function?
	MaxWalkSpeed = bIsJogging ? MaxSpeed_Jog : MaxSpeed_Walk;
	
}
