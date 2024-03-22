// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

// TODO: Is this needed?
void FBaseAnimInstanceProxy::SetVelocityData()
{
	OwnerAnimInstance->C_Velocity = CharacterMovement->Velocity;
	OwnerAnimInstance->C_Velocity2D = FVector(OwnerAnimInstance->C_Velocity.X, OwnerAnimInstance->C_Velocity.Y, 0.0f);
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
//
//void FBaseAnimInstanceProxy::Update(float DeltaSeconds)
//{
//	
//}
//
//void FBaseAnimInstanceProxy::PreUpdate(UAnimInstance* AnimInstance, float DeltaSeconds)
//{
//}
//
//void FBaseAnimInstanceProxy::PostUpdate(UAnimInstance* AnimInstance) const
//{
//}

void UShootyAnimInstance::SetVelocity()
{
	// Trace Tag for Animation Insights
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("Fancy move");

	Owner = Cast<AShooty>(GetOwningActor());
	if (!Owner)
		return;
	C_Velocity = FVector::ZeroVector;
	C_Velocity2D = FVector(C_Velocity.X, C_Velocity.Y, 0.0f);
	
}
