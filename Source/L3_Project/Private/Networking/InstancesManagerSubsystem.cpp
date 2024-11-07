// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/InstancesManagerSubsystem.h"

#include "Networking/BaseGameInstance.h"

int UInstancesManagerSubsystem::InstanceSessionID{};
bool UInstancesManagerSubsystem::IsInstanceBeingDestroyed{};

void UInstancesManagerSubsystem::StartNewInstance(int SessionID)
{
	UBaseGameInstance* GameInstance;
	if (!TryGetBaseGameInstance(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get game instance"));
		return;
	}
	
	if (!GameInstance->IsLoggedIn())
	{
		UE_LOG(LogTemp, Error, TEXT("Not logged in"));
		return;
	}

	auto OnTransition = [this, SessionID]() {
		UE_LOG(LogTemp, Log, TEXT("Transition completed. Starting new instance."));
		StartListenServer(SessionID);
	};
	
	auto OnSessionDestroyed = [this, GameInstance, OnTransition](const bool bWasSuccessful) {
		if(!bWasSuccessful)
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to destroy session. Going back to main menu."));

			// Go back to main menu
			return;
		}
		
		GameInstance->StartTransition(ENetTransitionType::LobbyToInstance, OnTransition);
	};
	
	GameInstance->DestroySessionWithCallback(OnSessionDestroyed);
}

void UInstancesManagerSubsystem::StartListenServer(const int SessionID) const
{
	if (UBaseGameInstance::HasRunningSession)
	{
		UE_LOG(LogTemp, Error, TEXT("Already in a session"));
		return;
	}

	if (UWorld* World = GetWorld(); World != nullptr)
	{
		InstanceSessionID = SessionID;

		const FString MapName = "/Game/ThirdPerson/Maps/ThirdPersonMap_Instance";
		const FURL ListenURL(nullptr, *(MapName + "?listen"), TRAVEL_Absolute);
		World->ServerTravel(ListenURL.ToString());
	}
}

void UInstancesManagerSubsystem::StopInstance()
{
}

void UInstancesManagerSubsystem::JoinInstance(FName SessionName, FBlueprintSessionSearchResult SessionData)
{
}

void UInstancesManagerSubsystem::ReturnToLobby()
{
}


