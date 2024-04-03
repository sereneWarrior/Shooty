// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstanceProxy.h"

#include "Shooty.h"

#include "ShootyAnimInstance.generated.h"

UENUM(Blueprinttype)
enum class E_MovementDirection : uint8
{
	Forewards   UMETA(DisplayName = "F"),
	Backwards   UMETA(DisplayName = "B"),
	Left		UMETA(DisplayName = "L"),
	Right		UMETA(DisplayName = "R"),
};


class UShootyAnimInstance;

// TODO: Remove Proxy and use NativeThreadSafeUpdateAnimation instead.
USTRUCT(BlueprintType)
struct SHOOTER_API FBaseAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_USTRUCT_BODY()

	void SetVelocityData();
protected:

	// Called on AnimInstance initialisation.
	virtual void Initialize(UAnimInstance* AnimInstance) override;
	// Runs on Anim thread, does calculations and updates varaibles inside proxy.
	virtual void Update(float DeltaSeconds) override;
	//// Called on gamethrerad before update, to copy any game data into proxy (anim instance, character, world etc.)
	//virtual void PreUpdate(UAnimInstance* AnimInstance, float DeltaSeconds) override;
	////Called on game thread, after update to copy updated data into anim instance
	//virtual void PostUpdate(UAnimInstance* AnimInstance) const override;

	//General
	UPROPERTY(Transient)
	UShootyAnimInstance* OwnerAnimInstance;

	UPROPERTY(Transient, BlueprintReadOnly)
	AShooty* Owner;

	UPROPERTY(Transient, BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovement;
};

UCLASS()
class SHOOTER_API UShootyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	
	friend struct FBaseAnimInstanceProxy;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FBaseAnimInstanceProxy Proxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return &Proxy; }
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	virtual void NativeBeginPlay() override;
	virtual void NativeThreadSafeUpdateAnimation(float _DeltaSeconds) override;
	// For test purpose:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(Transient, BlueprintReadOnly)
	AShooty* Owner;

	UPROPERTY(Transient, BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovement;

	// Locomotion
		// TODO: Still have another velocity in BP for testing.
	// Used for walking direction, not jumping!
	UPROPERTY(Transient, BlueprintReadOnly)
	FVector C_Velocity2D;

	UPROPERTY(Transient, BlueprintReadOnly)
	FRotator CharacterWorldRotation;

	// Set by evaluating Acceleration.
	UPROPERTY(Transient, BlueprintReadOnly)
	bool IsMoving;

	UPROPERTY(Transient, BlueprintReadOnly)
	E_MovementDirection MovementDirection;

	//  meta = (BlueprintThreadSafe) to call it in BP as Thread Safe animation
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CalculateMovementDirection(float locomationAngle, float FMin, float FMax, float BMin, float BMax);

private:
	FVector C_Acceleration2D;
	void UpdateVelocity();

	void UpdateCharacterWorldRotation();

	void UpdateMovementStatus();
};