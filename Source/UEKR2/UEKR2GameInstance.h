// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "Player/Inventory.h"
#include "UEKR2GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FMonsterTableInfo	:
	public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			TraceDistance;
};

USTRUCT(BlueprintType)
struct FPlayerTableInfo :
	public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPlayerJob		Job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			MoveSpeed;
};


UCLASS()
class UEKR2_API UUEKR2GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UUEKR2GameInstance();
	~UUEKR2GameInstance();

private:
	UPROPERTY()
		UDataTable* m_MonsterInfoTable;

	UPROPERTY()
		UDataTable* m_PlayerInfoTable;

	UPROPERTY()
		UDataTable* m_UIItemInfoTable;

	UPROPERTY()
		UDataTable* m_QuestInfoTable;

	EPlayerJob	m_SelectJob;

	FString		m_Name;

	Inventory*			m_Inventory;

	TArray<FPlayerInfo> m_PlayerInfo;

	TArray<UTexture2D*> m_PlayerCapture;

	TMap<int32, FQuestData> m_QuestCompleteMap;

public:
	UTexture2D* GetCapture(EPlayerJob Job)
	{
		if (Job != EPlayerJob::End)
			return m_PlayerCapture[(int32)Job];
		else
			return nullptr;
	}

	Inventory* GetInventory()
	{
		return m_Inventory;
	}

	const TMap<int32, FQuestData>& GetQuestData()
	{
		return m_QuestCompleteMap;
	}

	const FQuestData* FindQuestData(int32 Key)
	{
		return m_QuestCompleteMap.Find(Key);
	}

	EPlayerJob GetSelectJob()	const
	{
		return m_SelectJob;
	}

	FString GetPlayerName()
	{
		return m_Name;
	}

	FPlayerInfo* GetPlayerInfo()
	{
		return &m_PlayerInfo[(int)m_SelectJob];
	}

	FPlayerInfo* GetPlayerInfo(EPlayerJob Job)
	{
		return &m_PlayerInfo[(int)Job];
;	}

	void SetSelectJob(EPlayerJob Job)
	{
		m_SelectJob = Job;
	}

	void SetPlayerName(FString PlayerName)
	{
		m_Name = PlayerName;
	}

	void SavePlayerInfo(FPlayerInfo* Info)
	{
		m_PlayerInfo[(int)m_SelectJob] = *Info;
	}

	void SavePlayerInfo();

	void Reset();

public:
	virtual void Init();

public:
	const FMonsterTableInfo* FindMonsterInfo(int32 Number);
	const FUIItemTableInfo* FindUIItemInfo(int32 Number);
	const FQuestTableInfo* FindQuestInfo(int32 Number);

private:
	FPlayerTableInfo* FindPlayerInfo(int32 Number);

};
