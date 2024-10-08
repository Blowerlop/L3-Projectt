// Copyright Epic Games, Inc. All Rights Reserved.

#include "L3_ProjectGameMode.h"
#include "L3_ProjectPlayerController.h"
#include "L3_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AL3_ProjectGameMode::AL3_ProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AL3_ProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}