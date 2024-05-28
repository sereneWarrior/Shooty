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

protected:

	/*
	* Character related properties.
	*/
	UPROPERTY(BlueprintReadOnly)
	AShooty* Owner;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovement;

	/*
	* Locomotion system related
	*/

	// Used for walking direction, not jumping!
	UPROPERTY(BlueprintReadOnly, Category = "Velocity")
	FVector Velocity2D;

	UPROPERTY(BlueprintReadOnly)
	FRotator CharacterWorldRotation;

	// Set by evaluating Acceleration.
	UPROPERTY(BlueprintReadOnly)
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly)
	FVector RelativeAcceleration2D;

	UPROPERTY(BlueprintReadOnly)
	E_Direction MovementDirection;

	UPROPERTY(BlueprintReadOnly)
	EGait CurrentGait;

	UPROPERTY(BlueprintReadOnly)
	float LeanAngle;

	//____________________________________________________________________________

	//  meta = (BlueprintThreadSafe) to call it in BP as Thread Safe animation
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CalculateMovementDirection(float locomationAngle, float FMin, float FMax, float BMin, float BMax);

private:
	// Character Rotation for driving torso rotation according to controller movement during movement.
	void UpdateRotationAndProcessDelta(float _DeltaSeconds);
	void CalculateLeanAngle(double prev_yaw, float _DeltaSeconds);

	//Character Acceleration for driving Leaning motion.
	void UpdateVelocity(float _DeltaSeconds);
	void SetRelativeAcceleration(FVector delta_velocity, float _DeltaSeconds);
	FVector ClampAcceleration(FVector Velocity_delta, float AccelerationValue);

	void UpdateMovementStatus();
	
	
};