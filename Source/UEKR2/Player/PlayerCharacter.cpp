// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerAnim.h"
#include "Engine/TextureRenderTarget2D.h"
#include "../Effect/HitCameraShake.h"
#include "../Effect/NormalEffect.h"
#include "../UEKR2GameModeBase.h"
#include "../UEKR2GameInstance.h"
#include "../UEKR2SaveGame.h"
#include "../UI/MainHUD.h"
#include "../UI/CharacterHUD.h"
#include "../UI/HPBar.h"
#include "../UI/CoolTimeWidget.h"
#include "../UI/MinimapWidget.h"
#include "../Material/PhysicalMaterialBase.h"
#include "../Effect/GhostTrail.h"
#include "MainPlayerController.h"
#include "Grux.h"
#include "Phase.h"
#include "Shinbi.h"
#include "Wraith.h"

/*
언리얼엔진4에서 생성자는 에디터에서 게임을 실행할 경우 에디터에 배치되는 그 순간 호출된다.
에디터에서 게임을 플레이할때는 생성자가 호출이 안된다.

패키징된 최종 게임 실행파일로 실행을 한다면 생성자가 게임내에 배치될때 호출된다.
*/
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	m_HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	m_CaptureBody = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureBody"));
	m_Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));

	m_HPBar->SetupAttachment(GetMesh());
	m_Capture->SetupAttachment(GetMesh());
	m_CaptureBody->SetupAttachment(GetMesh());

	m_Trail->SetupAttachment(GetMesh());

	m_Arm->SetupAttachment(GetMesh());
	m_Arm->TargetArmLength = 900.f;
	m_Arm->TargetOffset = FVector(0.f, 0.f, 180.f);
	m_Arm->SetWorldRotation(FRotator(-15.f, 90.f, 0.f));
	m_Camera->SetupAttachment(m_Arm);

	GetCharacterMovement()->JumpZVelocity = 600.f;

	m_MoveKey = false;
	m_AttackEnable = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetMesh()->SetWorldLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetReceivesDecals(false);
	GetMesh()->SetCollisionProfileName(TEXT("Player"));
	SetCanBeDamaged(true);

	static ConstructorHelpers::FClassFinder<UUserWidget>	HPBarAsset(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));

	if (HPBarAsset.Succeeded())
		m_HPBar->SetWidgetClass(HPBarAsset.Class);

	m_HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	m_HPBar->SetDrawSize(FVector2D(200.f, 60.f));
	m_HPBar->SetRelativeLocation(FVector(0.f, 0.f, 230.f));
	m_HPBar->SetBlendMode(EWidgetBlendMode::Transparent);

	if (m_HPBarWidget)
		m_HPBarWidget->SetHPPercent((float)m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> FaceTargetObj(TEXT("TextureRenderTarget2D'/Game/Player/RTPlayerFace.RTPlayerFace'"));

	if (FaceTargetObj.Succeeded())
		m_FaceTarget = FaceTargetObj.Object;

	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	m_Capture->TextureTarget = m_FaceTarget;
	m_Capture->SetRelativeLocation(FVector(0.0f, 60.0f, 170.0f));
	m_Capture->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> BodyTargetObj(TEXT("TextureRenderTarget2D'/Game/Player/RTPlayerBody.RTPlayerBody'"));

	if (BodyTargetObj.Succeeded())
		m_BodyTarget = BodyTargetObj.Object;

	m_CaptureBody->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	m_CaptureBody->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	m_CaptureBody->TextureTarget = m_BodyTarget;
	m_CaptureBody->SetRelativeLocation(FVector(0.0f, 250.0f, 250.0f));
	m_CaptureBody->SetRelativeRotation(FRotator(-15.0f, -90.0f, 0.0f));

	m_Death = false;

	m_FallRecoveryMontage = nullptr;
	m_PlayerMesh = nullptr;

	m_ActionGhostTrail = false;
	m_OnGhostTrail = false;
	m_GhostTrailTime = 0.3f;
	m_GhostTrailTimeAcc = 0.0f;

	m_UseDash = true;
	m_UseDashTime = 0.f;
	m_DashCool = 5.f;

	m_UseSkill1 = true;
	m_UseSkill1Time = 0.f;
	m_Skill1Cool = 5.f;
	m_Skill1Power = 2.f;

	m_UseSkill2 = true;
	m_UseSkill2Time = 0.f;
	m_Skill2Cool = 7.f;
	m_Skill2Power = 3.f;

	m_UseSkill1MP = 50.f;
	m_UseSkill2MP = 100.f;

	m_Change = true;
	m_ChangeTime = 0.f;
	m_ChangeCool = 10.f;

	m_AttackBuffTime = 0.f;
	m_AttackBuff = 1.f;
	m_AttackBuffDur = 60.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	FPlayerInfo* Player = GameInst->GetPlayerInfo();

	m_PlayerInfo.Name = Player->Name;
	m_PlayerInfo.Job = Player->Job;
	m_PlayerInfo.Attack = Player->Attack;
	m_PlayerInfo.Armor = Player->Armor;
	m_PlayerInfo.HP = Player->HP;
	m_PlayerInfo.HPMax = Player->HPMax;
	m_PlayerInfo.MP = Player->MP;
	m_PlayerInfo.MPMax = Player->MPMax;
	m_PlayerInfo.AttackDistance = Player->AttackDistance;
	m_PlayerInfo.AttackSpeed = Player->AttackSpeed;
	m_PlayerInfo.AttackAngle = Player->AttackAngle;
	m_PlayerInfo.MoveSpeed = Player->MoveSpeed;

	USkeletalMeshComponent* MeshEx = GetMesh();

	m_AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	m_HPBarWidget = Cast<UHPBar>(m_HPBar->GetWidget());

	m_HPBarWidget->SetDelegate<APlayerCharacter>(this, &APlayerCharacter::NameWidgetCallback);

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				CharacterHUD->SetDelegate<APlayerCharacter>(this, &APlayerCharacter::CharacterHUDNameWidgetCallback);
			}
			
			UCoolTimeWidget* DashWidget = MainHUD->GetDashCoolWidget();

			if (IsValid(DashWidget))
			{
				DashWidget->SetTime(m_DashCool);
			}

			UCoolTimeWidget* Skill1Widget = MainHUD->GetSkill1CoolWidget();

			if (IsValid(Skill1Widget))
			{
				Skill1Widget->SetTime(m_Skill1Cool);
			}

			UCoolTimeWidget* Skill2Widget = MainHUD->GetSkill2CoolWidget();

			if (IsValid(Skill2Widget))
			{
				Skill2Widget->SetTime(m_Skill2Cool);
			}

			UCoolTimeWidget* ChangeWidget = MainHUD->GetChangeCoolWidget();

			if (IsValid(ChangeWidget))
			{
				ChangeWidget->SetTime(m_ChangeCool);
			}
			
			UCoolTimeWidget* BufWidget = MainHUD->GetBufCoolWidget();

			if (IsValid(BufWidget))
			{
				BufWidget->SetTime(m_AttackBuffTime);
			}
		}
	}

	m_Capture->ShowOnlyActors.Add(this);
	m_CaptureBody->ShowOnlyActors.Add(this);

	LOGSTRING(TestString);

	if (GameMode)
	{
		UMinimapWidget* Minimap = GameMode->GetMainHUD()->GetMinimap();

		if (Minimap)
		{
			Minimap->RotationArrow(GetActorRotation().Yaw);
		}


		GameMode->GetMainHUD()->GetInventoryList()->SetGold(m_PlayerInfo.Gold);
		GameMode->GetMainHUD()->GetInventoryTile()->SetGold(m_PlayerInfo.Gold);
	}

	GetWorldTimerManager().SetTimer(m_MonsterDetectTimer, this,
		&APlayerCharacter::MonsterDetect, 0.05f, true);


}

void APlayerCharacter::AddGold(int32 Gold)
{
	m_PlayerInfo.Gold += Gold;

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		GameMode->GetMainHUD()->GetInventoryList()->SetGold(m_PlayerInfo.Gold);
		GameMode->GetMainHUD()->GetInventoryTile()->SetGold(m_PlayerInfo.Gold);
	}
}

void APlayerCharacter::AddExp(int32 Exp)
{
	m_PlayerInfo.Exp += Exp;
}

void APlayerCharacter::AddHP(int32 HP)
{
	m_PlayerInfo.HP += HP;

	if (m_PlayerInfo.HP >= m_PlayerInfo.HPMax)
		m_PlayerInfo.HP = m_PlayerInfo.HPMax;

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				CharacterHUD->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
			}
		}
	}

	if (m_HPBarWidget)
		m_HPBarWidget->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
}

void APlayerCharacter::AddMP(int32 MP)
{
	m_PlayerInfo.MP += MP;

	if (m_PlayerInfo.MP >= m_PlayerInfo.MPMax)
		m_PlayerInfo.MP = m_PlayerInfo.MPMax;


	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				CharacterHUD->SetMPPercent(m_PlayerInfo.MP / (float)m_PlayerInfo.MPMax);
			}
		}
	}
}


//void APlayerCharacter::InvenInit()
//{
//
//	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());
//
//	if (GameMode)
//	{
//		// 현재는 list Tile 둘다 바인드 후에 인벤토리 통합 필요
//	}
//}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	UMainHUD* MainHUD = GameMode->GetMainHUD();

	if (IsValid(GameMode) && m_Death)
	{
		if (m_AnimInstance->IsDead())
			ChangeCharacter();
	}

	if (m_PlayerInfo.MP <= m_PlayerInfo.MPMax)
	{
		m_PlayerInfo.MP += 1.f * DeltaTime;
	}

	if (m_BoolAttackBuff)
	{
		m_AttackBuffTime += DeltaTime;

		MainHUD->GetBufCoolWidget()->SetTime(m_AttackBuffTime);

		if (m_AttackBuffTime >= m_AttackBuffDur)
		{
			m_BoolAttackBuff = false;
			m_AttackBuff = 1.f;
			m_AttackBuffTime = 0.f;
		}
	}

	if (!m_Change)
	{
		m_ChangeTime += DeltaTime;

		MainHUD->GetChangeCoolWidget()->SetTime(m_ChangeTime);

		if (m_ChangeTime >= m_ChangeCool)
		{
			m_Change = true;
			m_ChangeTime = 0.f;

		}
	}

	if (!m_UseDash)
	{
		m_UseDashTime += DeltaTime;
		
		MainHUD->GetDashCoolWidget()->SetTime(m_UseDashTime);

		if (m_UseDashTime >= m_DashCool)
		{
			m_UseDash = true;
			m_UseDashTime = 0.f;
		}
	}

	if (!m_UseSkill1)
	{
		m_UseSkill1Time += DeltaTime;

		MainHUD->GetSkill1CoolWidget()->SetTime(m_UseSkill1Time);

		if (m_UseSkill1Time >= m_Skill1Cool)
		{
			m_UseSkill1 = true;
			m_UseSkill1Time = 0.f;
		}
	}

	if (!m_UseSkill2)
	{
		m_UseSkill2Time += DeltaTime;

		MainHUD->GetSkill2CoolWidget()->SetTime(m_UseSkill2Time);

		if (m_UseSkill2Time >= m_Skill1Cool)
		{
			m_UseSkill2 = true;
			m_UseSkill2Time = 0.f;
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("RotationZ"), this,
		&APlayerCharacter::RotationZKey);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this,
		&APlayerCharacter::CameraZoomKey);
	PlayerInputComponent->BindAxis(TEXT("CameraLookUp"), this,
		&APlayerCharacter::CameraLookUpKey);

	PlayerInputComponent->BindAxis(TEXT("MoveFront"), this,
		&APlayerCharacter::MoveFrontKey);
	PlayerInputComponent->BindAxis(TEXT("MoveSide"), this,
		&APlayerCharacter::MoveSideKey);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, 
		&APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::AttackKey);
	PlayerInputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::Skill1Key);
	PlayerInputComponent->BindAction(TEXT("Skill2"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::Skill2Key);
	PlayerInputComponent->BindAction(TEXT("Potion1"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::UsePotion1Key);
	PlayerInputComponent->BindAction(TEXT("Potion2"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::UsePotion2Key);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("Save"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::GameSaveKey);

	PlayerInputComponent->BindAction(TEXT("Change"), EInputEvent::IE_Pressed, this,
		&APlayerCharacter::ChangeCharacter);

}


void APlayerCharacter::MoveFrontKey(float Scale)
{
	if (m_Death)
		return;

	AddMovementInput(GetActorForwardVector(), Scale);

	if (Scale == 0.f)
	{
		m_MoveKey = false;
		m_AnimInstance->SetDirection(0.f);
	}

	else if (Scale == 1.f)
	{
		m_MoveKey = true;
		m_AnimInstance->SetDirection(0.f);
	}

	else
	{
		m_MoveKey = true;
		m_AnimInstance->SetDirection(180.f);
	}
}

void APlayerCharacter::MoveSideKey(float Scale)
{
	if (m_Death)
		return;

	AddMovementInput(GetActorRightVector(), Scale);

	float	Direction = m_AnimInstance->GetDirection();

	if (Scale == 1.f)
	{
		m_MoveKey = true;
		if (Direction == 0.f)
			m_AnimInstance->SetDirection(45.f);

		else
			m_AnimInstance->SetDirection(135.f);
	}

	else if (Scale == -1.f)
	{
		m_MoveKey = true;
		if (Direction == 0.f)
			m_AnimInstance->SetDirection(-45.f);

		else
			m_AnimInstance->SetDirection(-135.f);
	}

}

void APlayerCharacter::RotationZKey(float Scale)
{
	if (m_Death)
		return;

	AddControllerYawInput(Scale);

	if (Scale != 0.f)
	{
		AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			UMinimapWidget* Minimap = GameMode->GetMainHUD()->GetMinimap();

			if (Minimap)
			{
				Minimap->RotationArrow(GetActorRotation().Yaw);
			}
		}
	}
}

void APlayerCharacter::CameraZoomKey(float Scale)
{
	m_Arm->TargetArmLength -= Scale * 20.f;

	if (m_Arm->TargetArmLength < 30.f)
		m_Arm->TargetArmLength = 30.f;

	else if (m_Arm->TargetArmLength > 900.f)
		m_Arm->TargetArmLength = 900.f;
}

void APlayerCharacter::CameraLookUpKey(float Scale)
{
	FRotator	Rot = m_Arm->GetRelativeRotation();

	Rot.Pitch += Scale * 10.f * GetWorld()->GetDeltaSeconds();

	if (Rot.Pitch > -15.f)
		Rot.Pitch = -15.f;

	else if (Rot.Pitch < -45.f)
		Rot.Pitch = -45.f;

	m_Arm->SetRelativeRotation(Rot);
}

void APlayerCharacter::JumpKey()
{
	if (m_Death)
		return;

	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Ground)
	{
		Jump();

		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Jump);
	}
}

void APlayerCharacter::AttackKey()
{
	if (m_Death)
		return;

	if (m_AttackEnable)
	{
		m_AttackEnable = false;
		Attack();
	}
}

void APlayerCharacter::Skill1Key()
{
	if (m_Death)
		return;

	if (m_UseSkill1MP <= m_PlayerInfo.MP && m_UseSkill1)
	{
		m_UseSkill1 = false;
		
		m_PlayerInfo.MP -= m_UseSkill1MP;

		m_AnimInstance->SetSkillUse(true);

		AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

		if (IsValid(GameMode))
		{
			UMainHUD* MainHUD = GameMode->GetMainHUD();

			if (IsValid(MainHUD))
			{
				UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

				if (IsValid(CharacterHUD))
					CharacterHUD->SetMPPercent(m_PlayerInfo.MP / (float)m_PlayerInfo.MPMax);
			}
		}

		Skill1();

	}
}

void APlayerCharacter::Skill2Key()
{
	if (m_Death)
		return;

	if (m_UseSkill2MP <= m_PlayerInfo.MP && m_UseSkill2)
	{
		m_UseSkill2 = false;
		
		m_PlayerInfo.MP -= m_UseSkill2MP;

		m_AnimInstance->SetSkillUse(true);

		AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

		if (IsValid(GameMode))
		{
			UMainHUD* MainHUD = GameMode->GetMainHUD();

			if (IsValid(MainHUD))
			{
				UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

				if (IsValid(CharacterHUD))
					CharacterHUD->SetMPPercent(m_PlayerInfo.MP / (float)m_PlayerInfo.MPMax);
			}
		}

		Skill2();

	}
}

void APlayerCharacter::DashKey()
{
	if (m_Death)
		return;

	Dash();
}

void APlayerCharacter::UsePotion1Key()
{
	if (m_Death)
		return;

	UsePotion(1);
}

void APlayerCharacter::UsePotion2Key()
{
	if (m_Death)
		return;

	UsePotion(2);
}


void APlayerCharacter::GameSaveKey()
{
	/*
	UUEKR2SaveGame* SaveGame = NewObject<UUEKR2SaveGame>();

	SaveGame->SetPlayerInfo(m_PlayerInfo);
	SaveGame->SetPos(GetActorLocation());
	SaveGame->SetRot(GetActorRotation());
	SaveGame->SetScale(GetActorScale());

	UGameplayStatics::SaveGameToSlot(SaveGame, "Save1", 0);
	*/

	/*PrintViewport(10.f, FColor::Black, FPaths::ProjectSavedDir());
	PrintViewport(10.f, FColor::Black, FPaths::ProjectContentDir());
	PrintViewport(10.f, FColor::Black, FPaths::ProjectConfigDir());*/

	FString FullPath = FString::Printf(TEXT("%s%s"), *FPaths::ProjectSavedDir(), TEXT("SavePlayer.txt"));

	FArchive* Writer = IFileManager::Get().CreateFileWriter(*FullPath);

	if (Writer)
	{
		*Writer << m_PlayerInfo.Name;
		*Writer << m_PlayerInfo.Job;
		*Writer << m_PlayerInfo.Attack;
		*Writer << m_PlayerInfo.Armor;
		*Writer << m_PlayerInfo.HP;
		*Writer << m_PlayerInfo.HPMax;
		*Writer << m_PlayerInfo.MP;
		*Writer << m_PlayerInfo.MPMax;
		*Writer << m_PlayerInfo.Level;
		*Writer << m_PlayerInfo.Exp;
		*Writer << m_PlayerInfo.Gold;
		*Writer << m_PlayerInfo.AttackDistance;
		*Writer << m_PlayerInfo.AttackSpeed;
		*Writer << m_PlayerInfo.AttackAngle;
		*Writer << m_PlayerInfo.MoveSpeed;

		FVector Pos, Scale;
		Pos = GetActorLocation();
		Scale = GetActorScale();
		FRotator Rot = GetActorRotation();

		*Writer << Pos;
		*Writer << Rot;
		*Writer << Scale;

		Writer->Close();

		delete Writer;

	}
}

void APlayerCharacter::PlayFallRecovery()
{
	/*
	if (!m_AnimInstance->Montage_IsPlaying(m_AnimInstance))
	{
		m_AnimInstance->Montage_SetPosition(m_FallRecoveryMontage, 0.f);
		m_AnimInstance->Montage_Play(m_FallRecoveryMontage);
	}*/
}

void APlayerCharacter::Attack()
{
	if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
	{
		m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_AttackIndex], 0.f);
		m_AnimInstance->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
		m_AnimInstance->SetAttackEnable(true);

		m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
	}
}

void APlayerCharacter::Skill1()
{
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[0]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[0], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[0]);
	}
}

void APlayerCharacter::Skill2()
{
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[1]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[1], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[1]);
	}
}

void APlayerCharacter::UsePotion(int32 PotionNumber)
{
	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		UUsePotionWidget* UsePotionWidget = GameMode->GetMainHUD()->GetUsePotionWidget();

		if (PotionNumber == 1)
			UsePotionWidget->UsePotion1();

		else if (PotionNumber == 2)
			UsePotionWidget->UsePotion2();
	}
}


void APlayerCharacter::Dash()
{
	if (m_UseDash)
	{
		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Dash);
		m_AnimInstance->Montage_SetPosition(m_DashMontage, 0.f);
		m_AnimInstance->Montage_Play(m_DashMontage);
		m_UseDash = false;

		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGhostTrail* Trail = GetWorld()->SpawnActor<AGhostTrail>(AGhostTrail::StaticClass(),
			GetMesh()->GetComponentLocation(),
			GetMesh()->GetComponentRotation(), param);

		Trail->SetMesh(m_PlayerMesh);
		Trail->SetColor(m_PlayerColor);
		Trail->CopyAnimation(GetMesh());

		FHitResult Hit;

		FCollisionQueryParams	params(NAME_None, false, this);

		bool Sweep = GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * 500,
			ECollisionChannel::ECC_GameTraceChannel1, params);
		
		if (Sweep)
			SetActorLocation(Hit.Location);

		else
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * 500);


#if ENABLE_DRAW_DEBUG

		//FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;

		//DrawDebugLine(GetWorld(), GetActorLocation(),
		//	GetActorLocation() + GetActorForwardVector() * 500, DrawColor, false, 1.f);

#endif

	}
}

void APlayerCharacter::NormalAttack()
{
	FVector	PlayerLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this);

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	TArray<FHitResult>	HitResultArray;
	bool Sweep = GetWorld()->SweepMultiByChannel(HitResultArray, PlayerLoc,
		PlayerLoc, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(m_PlayerInfo.AttackDistance),
		params);

	//LOG(TEXT("Attack : %.5f"), 200.f);
	//LOG(TEXT("TestAttack"));
	//PrintViewport(1.f, FColor::Yellow, TEXT("Attack"));

	TArray<FHitResult>	CollisionArray;
	if (Sweep)
	{
		// 1차 구충돌이 일어난 타겟과 나와의 각도를 구해준다.
		//for (int32 i = 0; i < HitResultArray.Num(); ++i)
		for (auto& result : HitResultArray)
		{
			FVector	Dir = result.ImpactPoint - GetActorLocation();

			// 구해준 방향벡터를 단위벡터로 만든다. 그 이유는 원뿔 형태로 체크를 하기 위해서
			// 각도를 체크해줘야 하는데 단위벡터의 내적을 이용하게 된다면 더 벡터간 각도의
			// 코사인 세타 값을 구해줄 수 있다.
			// 그러므로 이러한 코사인 세타를 아크코사인을 이용하여 세타(각도)로 변경하고
			// 이를 비교하여 원뿔 안에 들어오는지를 판단할 수 있다.
			Dir.Normalize();

			Forward = GetActorForwardVector();
			Forward.Normalize();

			float Dot = FVector::DotProduct(Dir, Forward);
			// Acos를 이용해서 각도를 구하면 이 값은 Radian 값으로 들어오게 된다.
			// 그러므로 이를 Degree 로 변환해주고 이를 비교하는 각도로 사용한다.
			float Angle = FMath::Acos(Dot);
			Angle = FMath::RadiansToDegrees(Angle);

			if (Angle <= m_PlayerInfo.AttackAngle)
				CollisionArray.Add(result);

			else
			{
				// 이게 아니라면 직선을 이용해서 상대방 충돌체를 관통하는지 판단한다.
			}
		}
	}
#if ENABLE_DRAW_DEBUG

	//FColor	DrawColor = CollisionArray.Num() > 0 ? FColor::Red : FColor::Green;

	////DrawDebugSphere(GetWorld(), PlayerLoc, m_PlayerInfo.AttackDistance, 20, DrawColor, false, 1.f);
	//DrawDebugCone(GetWorld(), PlayerLoc, GetActorForwardVector(), m_PlayerInfo.AttackDistance,
	//	FMath::DegreesToRadians(m_PlayerInfo.AttackAngle),
	//	FMath::DegreesToRadians(m_PlayerInfo.AttackAngle), 20,
	//	DrawColor, false, 1.f);

#endif

	for (auto& result : CollisionArray)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param);

		// 애셋을 로딩한다.
		//Effect->LoadParticle(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_thunder.P_ky_hit_thunder'"));
		//Effect->LoadParticleAsync(*m_AttackEffectString);
		LoadParticleEffect(Effect);

		// Sound 
		//Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
		Effect->LoadSoundAsync(TEXT("HitNormal"));

		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;

		float Damage = result.GetActor()->TakeDamage(m_PlayerInfo.Attack * m_AttackBuff, DmgEvent, GetController(), this);
	}
}

void APlayerCharacter::AttackEnd()
{
	m_AttackEnable = true;
}

void APlayerCharacter::UseSkill(int32 Index)
{
}

void APlayerCharacter::UseSkillFire(int32 Index)
{
}

void APlayerCharacter::GhostTrailEnd()
{
	m_ActionGhostTrail = false;
	m_GhostTrailTimeAcc = 0.0f;
}

void APlayerCharacter::OnGhostTrail()
{
	m_ActionGhostTrail = true;
	m_GhostTrailTimeAcc = 0.0f;
}

float APlayerCharacter::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Damage == 0.f)
		return 0.f;

	Damage = Damage - m_PlayerInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;

	m_PlayerInfo.HP -= Damage;

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (m_PlayerInfo.HP <= 0 && !m_Death)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		m_AnimInstance->ChangeAnimType(EPlayerAnimType::Death);
		m_Death = true;
	}

	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				CharacterHUD->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
			}
		}
	}

	if (IsValid(m_HPBarWidget))
	{
		m_HPBarWidget->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
	}

	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(
		UHitCameraShake::StaticClass());

	return Damage;
}

void APlayerCharacter::NameWidgetCallback()
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	m_HPBarWidget->SetName(GameInst->GetPlayerName());
}

void APlayerCharacter::CharacterHUDNameWidgetCallback()
{
	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (IsValid(MainHUD))
		{
			UCharacterHUD* CharacterHUD = MainHUD->GetCharacterHUD();

			if (IsValid(CharacterHUD))
			{
				CharacterHUD->SetName(m_PlayerInfo.Name);
			}
		}
	}
}

void APlayerCharacter::FootStep(bool Left)
{
	FVector SocketLoc;

	if (Left)
		SocketLoc = GetMesh()->GetSocketLocation(TEXT("Foot_L"));

	else
		SocketLoc = GetMesh()->GetSocketLocation(TEXT("Foot_R"));

	FCollisionQueryParams params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;

	FHitResult result;
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, SocketLoc, SocketLoc + FVector(0.0f, 0.0f, -100.0f),
		ECollisionChannel::ECC_EngineTraceChannel5, params);

	//bool Hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), );

	if (Hit)
	{
		UPhysicalMaterialBase* Phys = Cast<UPhysicalMaterialBase>(result.PhysMaterial);

		if (Phys)
		{
			PrintViewport(1.f, FColor::Red, TEXT("^.^"));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Phys->GetSound(), result.ImpactPoint);
		}
	}

	else
	{
		PrintViewport(1.f, FColor::Red, TEXT("T.T"));
	}
}

void APlayerCharacter::MonsterDetect()
{
	TArray<FHitResult> resultArray;

	FVector Pos = GetActorLocation();

	FCollisionQueryParams param(NAME_None, false, this);

	bool Collision = GetWorld()->SweepMultiByChannel(resultArray, Pos, Pos, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel11, FCollisionShape::MakeBox(FVector(4000.f, 4000.f, 1000.f)),
		param);

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());
	UMinimapWidget* Minimap = nullptr;

	if (GameMode)
	{
		Minimap = GameMode->GetMainHUD()->GetMinimap();

	}

	if (Minimap)
		Minimap->ClearMonster();

	if (Collision)
	{
		for (auto& result : resultArray)
		{
			FVector RelativePos = result.GetActor()->GetActorLocation() - Pos;

			if (Minimap)
			{
				Minimap->AddMonster(RelativePos);
			}
		}
	}
}

void APlayerCharacter::ChangeCharacter()
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

		if (IsValid(GameMode) && m_Change)
		{
			m_Change = false;

			GameInst->SavePlayerInfo(&m_PlayerInfo);

			GameMode->SpawnNextPlayer(this, m_PlayerInfo.Job);

			Destroy();
		}
	}
}

void APlayerCharacter::LoadParticleEffect(ANormalEffect* Effect)
{
	switch (m_PlayerInfo.Job)
	{
	case EPlayerJob::Knight:
		Effect->LoadParticleAsync(TEXT("GruxNormal"));
		break;
	case EPlayerJob::Archer:
		Effect->LoadParticleAsync(TEXT("WraithNormal"));
		break;
	case EPlayerJob::Magicion:
		Effect->LoadParticleAsync(TEXT("PhaseNormal"));
		break;
	case EPlayerJob::Assassin:
		Effect->LoadParticleAsync(TEXT("ShinbiNormal"));
		break;
	}
}

void APlayerCharacter::LoadDeathEffect()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(),GetActorRotation(), param);

	switch (m_PlayerInfo.Job)
	{
	case EPlayerJob::Knight:
		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonGrux/Characters/Heroes/Grux/Sounds/SoundWaves/Grux_Effort_Death_01.Grux_Effort_Death_01'"));
		break;
	case EPlayerJob::Archer:
		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonWraith/Audio/Wavs/Wraith_Effort_Death_010.Wraith_Effort_Death_010'"));
		break;
	case EPlayerJob::Magicion:
		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonPhase/Characters/Heroes/Phase/Sounds/SoundWaves/Phase_Effort_Death_01.Phase_Effort_Death_01'"));
		break;
	case EPlayerJob::Assassin:
		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Death_01.Shinbi_Effort_Death_01'"));
		break;
	}
}
