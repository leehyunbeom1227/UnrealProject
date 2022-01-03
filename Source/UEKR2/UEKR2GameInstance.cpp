// Fill out your copyright notice in the Description page of Project Settings.

#include "UEKR2GameInstance.h"
#include "UEKR2GameModeBase.h"
#include "Engine/Texture2D.h"
#include "Player/PlayerCharacter.h"
#include "UI/MainHUD.h"
#include "UI/OtherCharacterUI.h"

UUEKR2GameInstance::UUEKR2GameInstance()
{
	srand(time(NULL));

	static ConstructorHelpers::FObjectFinder<UDataTable>	MonsterInfoTableAsset(TEXT("DataTable'/Game/Monster/DTMonsterInfo.DTMonsterInfo'"));

	if (MonsterInfoTableAsset.Succeeded())
		m_MonsterInfoTable = MonsterInfoTableAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	PlayerInfoTableAsset(TEXT("DataTable'/Game/Player/DTPlayerInfo.DTPlayerInfo'"));

	if (PlayerInfoTableAsset.Succeeded())
		m_PlayerInfoTable = PlayerInfoTableAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	UIItemInfoTableAsset(TEXT("DataTable'/Game/Item/DTItemTable.DTItemTable'"));

	if (UIItemInfoTableAsset.Succeeded())
		m_UIItemInfoTable = UIItemInfoTableAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	QuestTableAsset(TEXT("DataTable'/Game/Quest/DTQuest.DTQuest'"));

	if (QuestTableAsset.Succeeded())
		m_QuestInfoTable = QuestTableAsset.Object;
	

	FString PathToLoad = "Texture2D'/Game/Capture/GruxFace.GruxFace'";
	UTexture2D* KnightTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *PathToLoad));

	m_PlayerCapture.Push(KnightTexture);

	PathToLoad = "Texture2D'/Game/Capture/WraithFace.WraithFace'";
	UTexture2D* ArcherTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *PathToLoad));

	m_PlayerCapture.Push(ArcherTexture);

	PathToLoad = "Texture2D'/Game/Capture/PhaseFace.PhaseFace'";
	UTexture2D* MagicionTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *PathToLoad));

	m_PlayerCapture.Push(MagicionTexture);
	
	PathToLoad = "Texture2D'/Game/Capture/ShinbiFace.ShinbiFace'";
	UTexture2D* AssassinTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *PathToLoad));

	m_PlayerCapture.Push(AssassinTexture);

	for (int i = 0; i < (int)EPlayerJob::End; ++i)
	{
		FPlayerTableInfo* TableInfo = FindPlayerInfo(i);	
		FPlayerInfo Playerinfo;
		
		Playerinfo.Key = TableInfo->Key;
		Playerinfo.Name = TableInfo->Name;
		Playerinfo.Job = TableInfo->Job;
		Playerinfo.Attack = TableInfo->Attack;
		Playerinfo.Armor = TableInfo->Armor;
		Playerinfo.HP = TableInfo->HP;
		Playerinfo.HPMax = TableInfo->HPMax;
		Playerinfo.MP = TableInfo->MP;
		Playerinfo.MPMax = TableInfo->MPMax;
		Playerinfo.Level = TableInfo->Level;
		Playerinfo.Exp = TableInfo->Exp;
		Playerinfo.Gold = TableInfo->Gold;
		Playerinfo.AttackDistance = TableInfo->AttackDistance;
		Playerinfo.AttackSpeed = TableInfo->AttackSpeed;
		Playerinfo.AttackAngle = TableInfo->AttackAngle;
		Playerinfo.MoveSpeed = TableInfo->MoveSpeed;

		m_PlayerInfo.Add(Playerinfo);
	}

	if (m_Inventory == nullptr)
		m_Inventory = new Inventory;
}

UUEKR2GameInstance::~UUEKR2GameInstance()
{
}

void UUEKR2GameInstance::Init()
{
	Super::Init();

}

void UUEKR2GameInstance::Reset()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
	
	m_SelectJob = EPlayerJob::Knight;

	for (int i = 0; i < (int)EPlayerJob::End; ++i)
	{
		FPlayerTableInfo* TableInfo = FindPlayerInfo(i);
		FPlayerInfo Playerinfo;

		Playerinfo.Key = TableInfo->Key;
		Playerinfo.Name = TableInfo->Name;
		Playerinfo.Job = TableInfo->Job;
		Playerinfo.Attack = TableInfo->Attack;
		Playerinfo.Armor = TableInfo->Armor;
		Playerinfo.HP = TableInfo->HP;
		Playerinfo.HPMax = TableInfo->HPMax;
		Playerinfo.MP = TableInfo->MP;
		Playerinfo.MPMax = TableInfo->MPMax;
		Playerinfo.Level = TableInfo->Level;
		Playerinfo.Exp = TableInfo->Exp;
		Playerinfo.Gold = TableInfo->Gold;
		Playerinfo.AttackDistance = TableInfo->AttackDistance;
		Playerinfo.AttackSpeed = TableInfo->AttackSpeed;
		Playerinfo.AttackAngle = TableInfo->AttackAngle;
		Playerinfo.MoveSpeed = TableInfo->MoveSpeed;

		m_PlayerInfo[i] = Playerinfo;
	}

	m_Inventory->Reset();

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		for (int i = 0; i < 3; ++i)
		{
			MainHUD->GetOtherCharacterUI(i + 1)->SetUI();
		}
	}

}

void UUEKR2GameInstance::SavePlayerInfo()
{
	m_PlayerInfo[(int)m_SelectJob] = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetPlayerInfo();
}

const FMonsterTableInfo* UUEKR2GameInstance::FindMonsterInfo(int32 Number)
{
	return m_MonsterInfoTable->FindRow<FMonsterTableInfo>(FName(*(FString::FormatAsNumber(Number))), "");
}

FPlayerTableInfo* UUEKR2GameInstance::FindPlayerInfo(int32 Number)
{
	return m_PlayerInfoTable->FindRow<FPlayerTableInfo>(FName(*(FString::FormatAsNumber(Number))), "");
}

const FUIItemTableInfo* UUEKR2GameInstance::FindUIItemInfo(int32 Number)
{
	return m_UIItemInfoTable->FindRow<FUIItemTableInfo>(FName(*(FString::FormatAsNumber(Number))), "");
}

const FQuestTableInfo* UUEKR2GameInstance::FindQuestInfo(int32 Number)
{
	return m_QuestInfoTable->FindRow<FQuestTableInfo>(FName(*(FString::FormatAsNumber(Number))), "");
}

