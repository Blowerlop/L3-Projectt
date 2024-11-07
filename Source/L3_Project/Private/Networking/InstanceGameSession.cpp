// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/InstanceGameSession.h"

#include "Networking/BaseGameInstance.h"


void AInstanceGameSession::HandleRegisterPlayerCompleted(FName EosSessionName, const TArray<FUniqueNetIdRef>& PlayerIds,
                                                         bool bWasSuccessful)
{
	Super::HandleRegisterPlayerCompleted(EosSessionName, PlayerIds, bWasSuccessful);
}

void AInstanceGameSession::HandleUnregisterPlayerCompleted(FName EosSessionName,
	const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccessful)
{
	Super::HandleUnregisterPlayerCompleted(EosSessionName, PlayerIds, bWasSuccessful);

	if (RegisteredPlayerCount == 1)
	{
		if (!UBaseGameInstance::IsInstanceBeingDestroyed) return;

		const auto GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
		if (!GameInstance) return;

		GameInstance->ReturnToLobby();
	}
}
