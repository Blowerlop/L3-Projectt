// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BaseGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FBlueprintSessionSearchResult
{
	GENERATED_USTRUCT_BODY()
	
	FOnlineSessionSearchResult OnlineResult;

	static FBlueprintSessionSearchResult GetFromCPP(const FOnlineSessionSearchResult& OnlineResult)
	{
		FBlueprintSessionSearchResult BlueprintResult;
		BlueprintResult.OnlineResult = OnlineResult;
		return BlueprintResult;
	}
};

UENUM(BlueprintType)
enum class EDisconnectType : uint8
{
	NetworkFailure UMETA(DisplayName="Network Failure"),
	BridgeMap UMETA(DisplayName="Bridge Map"), 
};

UENUM(BlueprintType)
enum class ENetTransitionType : uint8
{
	LobbyToInstance UMETA(DisplayName="Lobby To Instance"),
	InstanceToLobby UMETA(DisplayName="Instance To Lobby"), 
};

/**
 * 
 */
UCLASS()
class L3_PROJECT_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FFindSessionsDelegate, bool, bWasSuccessful, const FBlueprintSessionSearchResult&, Search);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FCreateSessionDelegate, FName, SessionName, bool, bWasSuccessful);
	
	DECLARE_DELEGATE_OneParam(FDestroySessionDelegate, bool);
	DECLARE_DELEGATE_TwoParams(FFindSessionsDelegateCPP, bool bWasSuccessful, const FBlueprintSessionSearchResult& Search);

	DECLARE_DELEGATE(FTransitionDelegate);
	
public:
	static bool IsSessionHost;
	static bool HasRunningSession;
	
	static int InstanceSessionID;
	static bool IsInstanceBeingDestroyed;
	
	static FName RunningSessionName;
	
	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void Login(bool bUseDevTool, FString AuthToolId = "") const;

	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void Logout() const;

	UFUNCTION(BlueprintCallable, Category = "Online Session")
	bool IsLoggedIn() const;
	
	UFUNCTION(BlueprintCallable, Category = "Custom Online Session", meta = (AutoCreateRefTerm = "Delegate"))
	void CreateSession(FName SessionName, FCreateSessionDelegate Delegate, FName KeyName = "KeyName",
		FString KeyValue = "KeyValue", bool bDedicatedServer = true);

	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void DestroySession();

	typedef std::function<void(const bool)> DSWCFunc;
	void DestroySessionWithCallback(const DSWCFunc& Func);
	
	UFUNCTION(BlueprintCallable, Category = "Custom Online Session", meta = (AutoCreateRefTerm = "Delegate"))
	void FindSessions(FName SearchKey, FString SearchValue, FFindSessionsDelegate Delegate);
	
	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void JoinSession(FName SessionName, FBlueprintSessionSearchResult SessionData);

	UFUNCTION(BlueprintCallable, Category = "Online Session")
	void StopInstance();
	
	UFUNCTION(BlueprintCallable, Category = "Online Session")
	void JoinInstance(FName SessionName, FBlueprintSessionSearchResult SessionData);

	void ReturnToLobby();

	typedef std::function<void()> TransitionFunc;
	void StartTransition(ENetTransitionType TransitionType);
	void StartTransition(ENetTransitionType TransitionType, const TransitionFunc& Func);
	
	UFUNCTION(BlueprintCallable, Category = "Online Session")
	void OnTransitionEntered();
	
private:
	FDelegateHandle CreateSessionDelegateHandle;
	FCreateSessionDelegate BP_CreateSessionDelegate;
	
	FDelegateHandle DestroySessionDelegateHandle;
	FDestroySessionDelegate DestroySessionDelegate;

	FDelegateHandle FindSessionsDelegateHandle;
	FFindSessionsDelegate BP_FindSessionsDelegate;
	FFindSessionsDelegateCPP CPP_FindSessionsDelegate;
	
	FDelegateHandle JoinSessionDelegateHandle;

	FTransitionDelegate TransitionDelegate;
	
	const int MaxNumberOfPlayersInSession = 5;

	virtual void Init() override;
	virtual void Shutdown() override;
	
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Arg, const FString& String);
	
	void HandleCreateSessionCompleted(FName EosSessionName, bool bWasSuccessful);
	
	void HandleDestroySessionCompleted(FName EosSessionName, bool bWasSuccessful);
	
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
