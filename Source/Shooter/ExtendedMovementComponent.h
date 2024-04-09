// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ExtendedMovementComponent.generated.h"


UCLASS()
class SHOOTER_API UExtendedMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	//TODO: Set backwards movement first.
	//TODO: Also set acceleration etc here.
	//TODO: Can I evaluate velocity directly here?
	UPROPERTY (Category = "Shooty Movement: Jog", EditAnywhere)
	float MaxSpeed_Jog = 750.0f;
	UPROPERTY(Category = "Shooty Movement: Jog", EditDefaultsOnly)
	float MaxBackwardsSpeed_Jog = 409.0f;
	UPROPERTY(Category = "Shooty Movement: Walk", EditDefaultsOnly)
	float MaxSpeed_Walk = 250.0f;
	UPROPERTY(Category = "Shooty Movement: Walk", EditDefaultsOnly)
	float MaxBackwardsSpeed_Walk = 45.0f;

	bool bIsJogging;
	bool bMoveBackwards;

public:
	UExtendedMovementComponent();

	/*Movement logic :*/ 
	void JogPressed();
	void JogReleased();
	
protected:
	/**
	 * Event triggered at the end of a movement update. If scoped movement updates are enabled (bEnableScopedMovementUpdates), this is within such a scope.
	 * If that is not desired, bind to the CharacterOwner's OnMovementUpdated event instead, as that is triggered after the scoped movement update.
	 */
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity);

};
