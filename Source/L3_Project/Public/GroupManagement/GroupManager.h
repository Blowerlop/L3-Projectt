#pragma once
#include "CoreMinimal.h"
#include "GroupManager.generated.h"

class ALobbyPlayerController;

USTRUCT(BlueprintType)
struct FInviteData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GroupId{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 InviteId{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> GroupMembers{};
};

class FServerGroupData
{
public:
	int32 GroupId{};
	
	TArray<ALobbyPlayerController*> GroupMembers{};

	void AddMember(ALobbyPlayerController* Player)
	{
		GroupMembers.Add(Player);
	}

	void RemoveMember(ALobbyPlayerController* Player)
	{
		GroupMembers.Remove(Player);
	}

	TArray<FString> GetMembersAsString() const;
};

USTRUCT(BlueprintType)
struct FReplicatedGroupData
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool IsValid = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GroupId{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> GroupMembers{};
};

class GroupManager
{
public:
	static TMap<int32, FServerGroupData> Groups;

	static int CreateGroup(ALobbyPlayerController* Owner);
	static void AddToGroup(ALobbyPlayerController* Player, int32 GroupId);
	static void RemoveFromGroup(ALobbyPlayerController* Player, int32 GroupId);
	static void DestroyGroup(int32 GroupId);
	
	static void InviteToGroup(ALobbyPlayerController* Inviter, ALobbyPlayerController* Invited);
	static void AcceptGroupInvite(ALobbyPlayerController* Invited, int32 InviteId);
	static bool IsGroupLeader(ALobbyPlayerController* Player);

	static FServerGroupData* GetGroup(int32 GroupId);

private:
	static int32 GroupIdCounter;
};
