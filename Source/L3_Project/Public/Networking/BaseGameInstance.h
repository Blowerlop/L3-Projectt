// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	
public:
	static bool IsSessionHost;
	static bool HasRunningSession;
	
	static int InstanceSessionID;
	
	static FName RunningSessionName;
	
	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void Login(bool bUseDevTool, FString AuthToolId = "") const;

	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void Logout() const;

	UFUNCTION(BlueprintCallable, Category = "Custom Online Session", meta = (AutoCreateRefTerm = "Delegate"))
	void CreateSession(FName SessionName, FCreateSessionDelegate Delegate, FName KeyName = "KeyName",
		FString KeyValue = "KeyValue", bool bDedicatedServer = true);

	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void DestroySession();
	void DestroySessionWithCallback(const FDestroySessionDelegate& Delegate);
	
	UFUNCTION(BlueprintCallable, Category = "Custom Online Session", meta = (AutoCreateRefTerm = "Delegate"))
	void FindSessions(FName SearchKey, FString SearchValue, FFindSessionsDelegate Delegate);
	
	UFUNCTION(BlueprintCallable, Category = "Custom Online Session")
	void JoinSession(FName SessionName, FBlueprintSessionSearchResult SessionData);

	UFUNCTION(BlueprintCallable, Category = "Online Session")
	bool IsLoggedIn() const;

private:
	FDelegateHandle CreateSessionDelegateHandle;
	FCreateSessionDelegate BP_CreateSessionDelegate;
	
	FDelegateHandle DestroySessionDelegateHandle;
	FDestroySessionDelegate DestroySessionDelegate;

	FDelegateHandle FindSessionsDelegateHandle;
	FFindSessionsDelegate BP_FindSessionsDelegate;
	FFindSessionsDelegateCPP CPP_FindSessionsDelegate;
	
	FDelegateHandle JoinSessionDelegateHandle;
	
	const int MaxNumberOfPlayersInSession = 5;

	virtual void Shutdown() override;
	
	void HandleCreateSessionCompleted(FName EosSessionName, bool bWasSuccessful);
	
	void HandleDestroySessionCompleted(FName EosSessionName, bool bWasSuccessful);
	
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
