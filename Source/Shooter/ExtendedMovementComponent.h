// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ExtendedMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FGaitSetting
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxBackwardsWalkSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAcceleration = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDeceleration = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFrictionFactor = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseSeperateBreakingFriction = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFriction = 0.0f;

};

// Values are set to work with the Leaning blendspace.
UENUM(Blueprinttype)
enum class EGait : uint8
{
	Walking  UMETA(DisplayName = "Walking"),
	Jogging  UMETA(DisplayName = "Jogging"),

};


UCLASS()
class SHOOTER_API UExtendedMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Shooty Movement", EditAnywhere)
	TMap<EGait, FGaitSetting> GaitSettings;

	bool bIsJogging;
	bool bIsMovingBackwards;

	float MaxBackwardsWalkSpeed;
	float MaxForewardsWalkSpeed;

public:
	UExtendedMovementComponent();

	/*Movement logic :*/ 
	void JogPressed();
	void JogReleased();
	void IsMovingBackwards(bool isMovingBackwards);

	// Update Movement Speed etc. responding to Gait set by character class.
	void SetGaitState(EGait movementStatus);
	
protected:
	/**
	 * Event triggered at the end of a movement update. If scoped movement updates are enabled (bEnableScopedMovementUpdates), this is within such a scope.
	 * If that is not desired, bind to the CharacterOwner's OnMovementUpdated event instead, as that is triggered after the scoped movement update.
	 */
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity);

};
