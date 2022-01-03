// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "../Effect/NormalEffect.h"
#include "../Effect/BossSkill1.h"
#include "../Effect/BossSkill2.h"
#include "../Effect/BossSkill3.h"
#include "../Effect/BossTeleport.h"
#include "../Effect/BossUltimateSkill.h"
#include "../Trigger/PortalTrigger.h"
#include "../UEKR2GameModeBase.h"
#include "../UEKR2GameInstance.h"
#include "../Item/ItemBox.h"
#include "../UI/MainHUD.h"
#include "../UI/BossHPBar.h"
#include "BossAIController.h"

// Sets default values
ABossMonster::ABossMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABossAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPSevarogAnim.BPSevarogAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("Sevarog");

	m_MonsterInfo.Key = (int32)EMonsterKey::Sevarog;

	m_Teleport = false;
	m_UseTeleport = false;

	m_FirstLoc = FVector();
}

// Called when the game starts or when spawned
void ABossMonster::BeginPlay()
{
	//GetMesh()->CreateDynamicMaterialInstance(0);
	Super::BeginPlay();

	GetMesh()->CreateDynamicMaterialInstance(0);
	
	m_FirstLoc = GetActorLocation();

	ChangeAnimType(EMonsterAnimType::Spawn);

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());
	
	m_HPBar = nullptr;

	if (GameMode)
	{
		GameMode->GetMainHUD()->ChangeBossHPBarType();
		m_BossHPBar = GameMode->GetMainHUD()->GetBossHPBar();
		m_BossHPBar->SetName(m_MonsterInfo.Name);
		m_BossHPBar->SetHPPercent(m_MonsterInfo.HP / (float)m_MonsterInfo.HPMax);
	}

}

// Called every frame
void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossMonster::SpawnAttack()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), GetActorRotation(), param);

	switch (m_AnimInstance->GetAnimType())
	{
	case EMonsterAnimType::Attack:
		switch (m_AnimInstance->GetAttackType())
		{
		case EMosnterAttackType::Attack1:
		{
			ABossSkill1* Attack = GetWorld()->SpawnActor<ABossSkill1>(ABossSkill1::StaticClass(),
				GetActorLocation() + GetActorForwardVector() * 50.f, GetActorRotation(), param);

			Attack->SetAttack(m_MonsterInfo.Attack);
			Attack->SetOwner(this);

			Effect->LoadSound(TEXT("SoundWave'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sounds/SoundWaves/Sevarog_Effort_Ability_Primary_01.Sevarog_Effort_Ability_Primary_01'"));
		}
		break;

		case EMosnterAttackType::Attack2:
		{
			ABossSkill2* Attack = GetWorld()->SpawnActor<ABossSkill2>(ABossSkill2::StaticClass(),
				GetActorLocation() + GetActorForwardVector() * 50.f, GetActorRotation(), param);

			Attack->SetAttack(m_MonsterInfo.Attack * 2);
			Attack->SetOwner(this);
			
			Effect->LoadSound(TEXT("SoundWave'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sounds/SoundWaves/Sevarog_Effort_Ability_Primary_02.Sevarog_Effort_Ability_Primary_02'"));
		}
		break;

		case EMosnterAttackType::Attack3:
		{
			ABossSkill3* Attack = GetWorld()->SpawnActor<ABossSkill3>(ABossSkill3::StaticClass(),
				GetActorLocation() + GetActorForwardVector() * 50.f, GetActorRotation(), param);

			Attack->SetAttack(m_MonsterInfo.Attack * 3);
			Attack->SetOwner(this);

			Effect->LoadSound(TEXT("SoundWave'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sounds/SoundWaves/Sevarog_Effort_Ability_Primary_03.Sevarog_Effort_Ability_Primary_03'"));

		}
		break;

		case EMosnterAttackType::Attack4:
		{
			ABossUltimateSkill* Skill = GetWorld()->SpawnActor<ABossUltimateSkill>(ABossUltimateSkill::StaticClass(),
				GetActorLocation() + GetActorForwardVector() * 50.f, GetActorRotation(), param);

			Skill->SetAttack(m_MonsterInfo.Attack * 5);
			Skill->SetOwner(this);

			Effect->LoadSound(TEXT("SoundWave'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sounds/SoundWaves/Sevarog_Effort_Ability_Ultimate_03.Sevarog_Effort_Ability_Ultimate_03'"));
		}
		break;
		}
		break;

	case EMonsterAnimType::Skill:
	{
		ABossUltimateSkill* Skill = GetWorld()->SpawnActor<ABossUltimateSkill>(ABossUltimateSkill::StaticClass(),
			GetActorLocation() + GetActorForwardVector() * 50.f, GetActorRotation(), param);

		Skill->SetAttack(m_MonsterInfo.Attack * 5);
		Skill->SetOwner(this);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sounds/SoundWaves/Sevarog_Effort_Ability_Ultimate_03.Sevarog_Effort_Ability_Ultimate_03'"));
	}
	break;
	}

}

void ABossMonster::Teleport()
{
	FVector Loc = m_FirstLoc;

	FBox	SpawnBox(m_FirstLoc - FVector(500.f, 500.f, 0.f),
		m_FirstLoc + FVector(500.f, 500.f, 0.f));
	
	FVector RandPoint = FMath::RandPointInBox(SpawnBox);
	
	SetActorLocation(RandPoint);

	FVector AfterLoc = RandPoint;

	FVector Dir = AfterLoc - Loc;

	Dir.Normalize();
	
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ABossTeleport>(ABossTeleport::StaticClass(),
		Loc, Dir.Rotation(), param);

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), GetActorRotation(), param);

	Effect->LoadSound(TEXT("SoundWave'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sounds/SoundWaves/Sevarog_Effort_JumpHeavy_01.Sevarog_Effort_JumpHeavy_01'"));

	m_Teleport = false;
	m_UseTeleport = true;
}

float ABossMonster::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Damage == 0.f)
		return 0.f;

	Damage = Damage - m_MonsterInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;

	m_MonsterInfo.HP -= Damage;

	// 몬스터가 죽었을 경우
	if (m_MonsterInfo.HP <= 0)
	{
		m_AnimInstance->ChangeAnimType(EMonsterAnimType::Death);

		AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

		if (MonsterController)
			MonsterController->BrainComponent->StopLogic(TEXT("Dead"));

		AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			UQuestWidget* QuestWidget = GameMode->GetMainHUD()->GetQuest();

			if (QuestWidget)
			{
				QuestWidget->QuestCheck(EQuestType::Hunt, m_MonsterInfo.Name);
			}
		}

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		float Percent = FMath::RandRange(0.f, 100.f);

		if (Percent <= 90.f)
		{
			int32 Index = FMath::RandRange(0, m_DropItemKeyArray.Num() - 1);

			UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

			if (GameInst)
			{
				const FUIItemTableInfo* ItemInfo = GameInst->FindUIItemInfo(m_DropItemKeyArray[Index]);

				if (ItemInfo)
				{
					FActorSpawnParameters param;
					param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					AItemBox* ItemBox = GetWorld()->SpawnActor<AItemBox>(AItemBox::StaticClass(),
						GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 50.f),
						GetMesh()->GetComponentRotation(), param);

					if (ItemBox)
					{
						ItemBox->SetMesh(ItemInfo->Mesh);
						ItemBox->SetKey(m_DropItemKeyArray[Index]);

						PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("DropItem : %s"), *ItemInfo->Name));
					}
				}
			}
		}
	}

	else
		m_AnimInstance->SetHit();

	if (IsValid(m_BossHPBar))
		m_BossHPBar->SetHPPercent(m_MonsterInfo.HP / (float)m_MonsterInfo.HPMax);

	return Damage;
}

void ABossMonster::Death()
{
	Super::Death();
	
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		GetActorLocation(), GetActorRotation(), param);

	Effect->LoadSound(TEXT("SoundWave'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sounds/SoundWaves/Sevarog_Effort_Death_01.Sevarog_Effort_Death_01'"));

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());
	
	m_Portal->LinkPortal();

	if(GameMode)
		GameMode->GetMainHUD()->ChangeBossHPBarType();
}
