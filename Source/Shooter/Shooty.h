// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "Shooty.generated.h"

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
	float BrakingDeceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFrictionFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseSeperateBreakingFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFriction;

	FORCEINLINE uint32 GetTypeHash(const  FGaitSetting& This)
	{
		return HashCombine(GetTypeHash(This), sizeof(FGaitSetting));
	}
};

UENUM(Blueprinttype)
enum class EGait : uint8
{
	Walking   UMETA(DisplayName = "Walking"),
	Jogging   UMETA(DisplayName = "Jogging"),

};


UCLASS(config=Game)
class SHOOTER_API AShooty : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Jog Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JogAction;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Body;

	// TODO: My BW Animation is slower than my FW animation. Should this be handled in AnimBP?
	// TODO: Value are now part of GaitSetting struct. Can be removed.
	UPROPERTY(Category = Movement, EditAnywhere)
	float BackwardsWalkingSpeed = 45.0f;

	UPROPERTY(Category = Movement, EditAnywhere)
	float ForwardWalkingSpeed = 250.0f;

public:
	// Sets default values for this character's properties
	AShooty();

	/*CAMERA*/
	UPROPERTY(Category = Camera, EditAnywhere)
	float ViewPitchMax = 40.0f;

	UPROPERTY(Category = Camera, EditAnywhere)
	float ViewPitchMin = -40.0f;

	/*GAIT SETTINGS*/
	UPROPERTY(Category = Movement, BlueprintReadWrite)
	TMap<EGait, FGaitSetting> GaitSettings;

	UPROPERTY(Category = Movement, BlueprintReadWrite)
	EGait CurrentGait;

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void UpdateGait(const FInputActionValue& Value, const EGait newGait);
};
