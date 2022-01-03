// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/SaveGame.h"
#include "UEKR2SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UUEKR2SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UUEKR2SaveGame();

private:
	UPROPERTY()
		FPlayerInfo m_PlayerInfo;

	UPROPERTY()
		FVector m_Pos;

	UPROPERTY()
		FVector m_Scale;

	UPROPERTY()
		FRotator m_Rot;

public:
	void SetPlayerInfo(const FPlayerInfo& Info)
	{
		m_PlayerInfo = Info;
	}

	void SetPos(const FVector& Pos)
	{
		m_Pos = Pos;
	}

	void SetScale(const FVector& Scale)
	{
		m_Scale = Scale;
	}

	void SetRot(const FRotator& Rot)
	{
		m_Rot = Rot;
	}

	const FPlayerInfo& GetPlayerInfo() const
	{
		return m_PlayerInfo;
	}

	const FVector& GetPos() const
	{
		return m_Pos;
	}

	const FVector& GetScale() const
	{
		return m_Scale;
	}

	const FRotator& GetRot() const
	{
		return m_Rot;
	}
};
