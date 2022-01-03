// Copyright Epic Games, Inc. All Rights Reserved.


#include "UEKR2GameModeBase.h"
#include "UEKR2GameInstance.h"
#include "Player/MainPlayerController.h"
#include "Player/PlayerCharacter.h"
#include "Player/Grux.h"
#include "Player/Phase.h"
#include "Player/Wraith.h"
#include "Player/Shinbi.h"
#include "UI/MainHUD.h"
#include "UI/OtherCharacterUI.h"

AUEKR2GameModeBase::AUEKR2GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UMainHUD>	MainHUDClass(TEXT("WidgetBlueprint'/Game/UI/UI_MainHUD.UI_MainHUD_C'"));

	if (MainHUDClass.Succeeded())
		m_MainHUDClass = MainHUDClass.Class;

	static ConstructorHelpers::FClassFinder<APawn>	KnightClass(TEXT("Blueprint'/Game/Player/Grux/BPGrux.BPGrux_C'"));

	if (KnightClass.Succeeded())
		m_PlayerClassArray.Add(KnightClass.Class);

	static ConstructorHelpers::FClassFinder<APawn>	ArcherClass(TEXT("Blueprint'/Game/Player/Wraith/BPWraith.BPWraith_C'"));

	if (ArcherClass.Succeeded())
		m_PlayerClassArray.Add(ArcherClass.Class);

	static ConstructorHelpers::FClassFinder<APawn>	MagicionClass(TEXT("Blueprint'/Game/Player/Phase/BPPhase.BPPhase_C'"));

	if (MagicionClass.Succeeded())
		m_PlayerClassArray.Add(MagicionClass.Class);

	static ConstructorHelpers::FClassFinder<APawn>	AssassinClass(TEXT("Blueprint'/Game/Player/Shinbi/BPShinbi.BPShinbi_C'"));

	if (AssassinClass.Succeeded())
		m_PlayerClassArray.Add(AssassinClass.Class);

	PlayerControllerClass = AMainPlayerController::StaticClass();
}

void AUEKR2GameModeBase::InitGame(const FString& MapName, 
	const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		EPlayerJob SelectJob = GameInst->GetSelectJob();
		DefaultPawnClass = m_PlayerClassArray[(int32)SelectJob];
		GameInst->SetSelectJob(SelectJob);
	}

	m_MainAssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();
	m_MainAssetPath->ConvertPath();
}

void AUEKR2GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_MainHUDClass))
	{
		m_MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(),	m_MainHUDClass));

		if (m_MainHUD)
			m_MainHUD->AddToViewport();
	}

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	FInputModeGameAndUI	Mode;
	//FInputModeGameOnly
	//FInputModeGameAndUI

	Controller->SetInputMode(Mode);
	Controller->bShowMouseCursor = true;
}

void AUEKR2GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

bool AUEKR2GameModeBase::SpawnNextPlayer(APlayerCharacter* Player, EPlayerJob Job)
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	EPlayerJob NextJob = Job;
	int i = 0;

	do
	{
		if (GameInst->GetPlayerInfo()->HP <= 0)
			++i;

		if (i == (int)EPlayerJob::End)
		{
			GameInst->Reset();
		
			return false;
		}

		NextJob = ++NextJob;
		GameInst->SetSelectJob(NextJob);

	} while (GameInst->GetPlayerInfo()->HP <= 0);

	APlayerController* NextPlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GameInst->GetPlayerInfo();

	switch (NextJob)
	{
	case::EPlayerJob::Knight:
	{
		GameInst->SetPlayerName("Grux");
		GameInst->SetSelectJob(EPlayerJob::Knight);

		AGrux* NextPlayer = GetWorld()->SpawnActor<AGrux>(AGrux::StaticClass(),
			Player->GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 44.f),
			Player->GetMesh()->GetComponentRotation() + FRotator(0.f, 90.f, 0.f)
			, param);

		NextPlayer->SetAttackBuff(Player->GetAttackBuf(), Player->GetAttackBufTime());
		NextPlayer->SetDashTime(Player->GetUseDash(), Player->GetUseDashTime());
		NextPlayer->SetChangeFalse();
		NextPlayerController->Possess(NextPlayer);		

	}
	break;
	case::EPlayerJob::Archer:
	{
		GameInst->SetPlayerName("Wraith");
		GameInst->SetSelectJob(EPlayerJob::Archer);

		AWraith* NextPlayer = GetWorld()->SpawnActor<AWraith>(AWraith::StaticClass(),
			Player->GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 44.f),
			Player->GetMesh()->GetComponentRotation() + FRotator(0.f, 90.f, 0.f)
			, param);

		NextPlayer->SetAttackBuff(Player->GetAttackBuf(), Player->GetAttackBufTime());
		NextPlayer->SetDashTime(Player->GetUseDash(), Player->GetUseDashTime());
		NextPlayer->SetChangeFalse();		
		NextPlayerController->Possess(NextPlayer);
	}
	break;
	case::EPlayerJob::Magicion:
	{
		GameInst->SetPlayerName("Phase");
		GameInst->SetSelectJob(EPlayerJob::Magicion);

		APhase* NextPlayer = GetWorld()->SpawnActor<APhase>(APhase::StaticClass(),
			Player->GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 44.f),
			Player->GetMesh()->GetComponentRotation() + FRotator(0.f, 90.f, 0.f)
			, param);

		NextPlayer->SetAttackBuff(Player->GetAttackBuf(), Player->GetAttackBufTime());
		NextPlayer->SetDashTime(Player->GetUseDash(), Player->GetUseDashTime());
		NextPlayer->SetChangeFalse();
		NextPlayerController->Possess(NextPlayer);

	}
	break;
	case::EPlayerJob::Assassin:
	{
		GameInst->SetPlayerName("Shinbi");
		GameInst->SetSelectJob(EPlayerJob::Assassin);

		AShinbi* NextPlayer = GetWorld()->SpawnActor<AShinbi>(AShinbi::StaticClass(),
			Player->GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 44.f),
			Player->GetMesh()->GetComponentRotation() + FRotator(0.f, 90.f, 0.f)
			, param);

		NextPlayer->SetAttackBuff(Player->GetAttackBuf(), Player->GetAttackBufTime());
		NextPlayer->SetDashTime(Player->GetUseDash(), Player->GetUseDashTime());
		NextPlayer->SetChangeFalse();
		NextPlayerController->Possess(NextPlayer);

	}
	break;
	}

	UCharacterHUD* CharacterHUD = m_MainHUD->GetCharacterHUD();

	if (IsValid(CharacterHUD))
	{
		APlayerCharacter* Player =	Cast<APlayerCharacter>(NextPlayerController->GetPawn());
		const FPlayerInfo& PlayerInfo = Player->GetPlayerInfo();
		CharacterHUD->SetName(PlayerInfo.Name);
		CharacterHUD->SetMPPercent((float)PlayerInfo.MP / (float)PlayerInfo.MPMax);
		CharacterHUD->SetHPPercent((float)PlayerInfo.HP / (float)PlayerInfo.HPMax);

	}
	
	for (int j = 0; j < 3; ++j)
	{
		NextJob = ++NextJob;
		UOtherCharacterUI* OtherCharacterUI = m_MainHUD->GetOtherCharacterUI(j + 1);
		OtherCharacterUI->SetJob(NextJob);
		OtherCharacterUI->SetUI();
	}

	return true;
}