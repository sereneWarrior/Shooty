// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Shooty.h"
#include "Gait.h"

#include "ShootyAnimInstance.generated.h"


UENUM(Blueprinttype)
enum class E_Direction : uint8
{
	Forewards   UMETA(DisplayName = "F"),
	Backwards   UMETA(DisplayName = "B"),
	Left		UMETA(DisplayName = "L"),
	Right		UMETA(DisplayName = "R"),
	LeftBW		UMETA(DisplayName = "LB"),
	RightBW		UMETA(DisplayName = "RB"),
};

UCLASS()
class SHOOTER_API UShootyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void ReceiveGaitStatus(EGait gaitStatus);

protected:
	
	virtual void NativeBeginPlay() override;
	virtual void NativeThreadSafeUpdateAnimation(float _DeltaSeconds) override;
	// For test purpose:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(Transient, BlueprintReadOnly)
	AShooty* Owner;

	UPROPERTY(Transient, BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovement;

	// Locomotion
	
	// Used for walking direction, not jumping!
	UPROPERTY(Transient, BlueprintReadOnly)
	FVector C_Velocity2D;	// TODO: Still have another velocity in BP for testing.

	UPROPERTY(Transient, BlueprintReadOnly)
	FRotator CharacterWorldRotation;

	// Set by evaluating Acceleration.
	UPROPERTY(Transient, BlueprintReadOnly)
	bool IsMoving;

	UPROPERTY(Transient, BlueprintReadOnly)
	E_Direction MovementDirection;

	UPROPERTY(BlueprintReadOnly)
	EGait CurrentGait;

	//  meta = (BlueprintThreadSafe) to call it in BP as Thread Safe animation
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CalculateMovementDirection(float locomationAngle, float FMin, float FMax, float BMin, float BMax);
private:
	FVector C_Acceleration2D;

	void UpdateVelocity();

	void UpdateCharacterWorldRotation();

	void UpdateMovementStatus();
};