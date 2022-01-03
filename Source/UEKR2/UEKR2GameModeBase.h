// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameInfo.h"
#include "UI/MainHUD.h"
#include "AssetManager/AssetPathMain.h"
#include "GameFramework/GameModeBase.h"
#include "UEKR2GameModeBase.generated.h"

UCLASS()
class UEKR2_API AUEKR2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AUEKR2GameModeBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainHUD>	m_MainHUDClass;

	TArray<TSubclassOf<APawn>> m_PlayerClassArray;

	UMainHUD*		m_MainHUD;
	UAssetPathMain* m_MainAssetPath;

public:
	UMainHUD* GetMainHUD()
	{
		return m_MainHUD;
	}

public:
	bool SpawnNextPlayer(class APlayerCharacter* Player, EPlayerJob Job);

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
