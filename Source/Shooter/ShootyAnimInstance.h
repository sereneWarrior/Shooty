// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstanceProxy.h"

#include "Shooty.h"

#include "ShootyAnimInstance.generated.h"
class UShootyAnimInstance;

USTRUCT(BlueprintType)
struct SHOOTER_API FBaseAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_USTRUCT_BODY()

	void SetVelocityData();
protected:

	// Called on AnimInstance initialisation.
	virtual void Initialize(UAnimInstance* AnimInstance) override;
	// Runs on Anim thread, does calculations and updates varaibles inside proxy.
	//virtual void Update(float DeltaSeconds) override;
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

	//Movement
	UPROPERTY(Transient, BlueprintReadOnly)
	float Speed;

	UPROPERTY(Transient, BlueprintReadOnly)
	FVector Acceleration;

	
};

//UCLASS()
//class SHOOTER_API UShootyAnimInstance : public UAnimInstance
//{
//	GENERATED_BODY()
//public:
//	friend struct FBaseAnimInstanceProxy;
//
//protected:
//	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
//	FBaseAnimInstanceProxy Proxy;
//
//	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return &Proxy; }
//	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
//
//	UPROPERTY(Transient, BlueprintReadOnly)
//	AShooty* Owner;
//
//	// Movement
//	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
//	float Speed;
//
//	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
//	FVector 
//	C_Velocity;
//	UPROPERTY(Transient, BlueprintReadOnly)
//	FVector C_Velocity2D;
//
//	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
//	FVector Acceleration;
//
//	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
//	void SetVelocity();
//};

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

	//virtual void NativeUpdateAnimation(float _DeltaSeconds) override;

	//virtual void NativeThreadSafeUpdateAnimation(float _DeltaSeconds) override;


	//virtual void NativePostEvaluateAnimation() override;


	UPROPERTY(Transient, BlueprintReadOnly)
	AShooty* Owner;

	//TODO: Integrate into BP.
	// Movement
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	float Speed;

	// TODO: Still have another velocity in BP for testing.
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
	FVector C_Velocity;
	UPROPERTY(Transient, BlueprintReadOnly)
	FVector C_Velocity2D;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	FVector Acceleration;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetVelocity();
};