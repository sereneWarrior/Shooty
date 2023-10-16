// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Shooty.h"

AShooterGameMode::AShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/MetaHumans/Bernice/BP_Bernice.BP_Bernice_C'"));
	UE_LOG(LogTemp, Warning, TEXT("Setup "));
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		
		
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
