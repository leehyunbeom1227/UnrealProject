// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionPrime.h"
#include "../Effect/NormalEffect.h"
#include "../Effect/PrimeAttack1.h"
#include "../Effect/PrimeAttack2.h"
#include "../Effect/PrimeAttack3.h"
#include "../Effect/PrimeSkill.h"
#include "../Trigger/PortalTrigger.h"
#include "../UEKR2GameModeBase.h"
#include "../UEKR2GameInstance.h"
#include "../Item/ItemBox.h"
#include "../UI/MainHUD.h"
#include "../UI/BossHPBar.h"
#include "MinionPrimeAIController.h"
#include "MonsterAnimInstance.h"
#include "Monster.h"

// Sets default values
AMinionPrime::AMinionPrime()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMinionPrimeAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Prime_Helix/Meshes/Prime_Helix.Prime_Helix'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPMinionPrimeAnim.BPMinionPrimeAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("MinionPrime");
	m_MonsterInfo.Key = (int32)EMonsterKey::MinionPrime;

	m_TurnFloat = 0.f;
	m_WaitTime = 5.f;
	m_CanAttack = true;
}

// Called when the game starts or when spawned
void AMinionPrime::BeginPlay()
{
	Super::BeginPlay();

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
void AMinionPrime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_CanAttack)
	{
		m_AccTime += DeltaTime;

		if (m_AccTime >= m_WaitTime)
		{
			m_AccTime = 0.f;
			m_CanAttack = true;
		}
	}

	if (m_AnimInstance->GetAnimType() == EMonsterAnimType::Turn)
	{
		FVector PlayerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FVector	Dir = PlayerLoc - GetActorLocation();
		Dir.Z = 0.f;
		Dir.Normalize();

		FVector ForwardVector = GetActorForwardVector();
		ForwardVector.Z = 0.f;
		ForwardVector.Normalize();

		float Dirdot = FVector::DotProduct(ForwardVector, Dir);
		float Dirangle = FMath::RadiansToDegrees(FMath::Acos(Dirdot));

		if (abs(Dirangle) < 1.f)
		{
			m_AnimInstance->ChangeEndAnimType(EMonsterAnimType::Turn);
			m_TurnFloat = 0.f;

			return;
		}

		float dot = FVector::DotProduct(GetActorForwardVector(), Dir);
		float angle = FMath::RadiansToDegrees(FMath::Acos(dot));

		if (m_TurnFloat == 0.f)
			m_TurnFloat = angle;

		if (GetAnim()->GetTurnType() == EMonsterTurnType::Left)
			AddActorLocalRotation(FRotator(0.f, m_TurnFloat * DeltaTime, 0.f));
		else
			AddActorLocalRotation(FRotator(0.f, -m_TurnFloat * DeltaTime, 0.f));
	}

}

void AMinionPrime::SpawnPrimeAttack()
{
	FVector	AimLoc = GetMesh()->GetSocketLocation(TEXT("Aim_Target"));
	
	//GetActorLocation() + GetActorForwardVector() * 50.f - FVector(0.f, 0.f, 10.f)
	switch (m_AnimInstance->GetAnimType())
	{
	case EMonsterAnimType::Attack:
		switch (m_AnimInstance->GetAttackType())
		{
		case EMosnterAttackType::Attack1:
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			APrimeAttack1* Attack = GetWorld()->SpawnActor<APrimeAttack1>(APrimeAttack1::StaticClass(),
				AimLoc, GetActorRotation(), param);

			Attack->SetAttack(m_MonsterInfo.Attack);
			Attack->SetOwner(this);
		}
			break;

		case EMosnterAttackType::Attack2:
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			APrimeAttack2* Attack = GetWorld()->SpawnActor<APrimeAttack2>(APrimeAttack2::StaticClass(),
				AimLoc, GetActorRotation(), param);

			Attack->SetAttack(m_MonsterInfo.Attack * 2);
			Attack->SetOwner(this);
		}
			break;

		case EMosnterAttackType::Attack3:
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			APrimeAttack3* Attack = GetWorld()->SpawnActor<APrimeAttack3>(APrimeAttack3::StaticClass(),
				GetActorLocation() + GetActorForwardVector() * 50 - FVector(0.f,0.f,10.f), GetActorRotation(), param);

			Attack->SetAttack(m_MonsterInfo.Attack * 3);
			Attack->SetOwner(this);
		}
			break;
		}
		break;

	case EMonsterAnimType::Skill:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APrimeSkill* Skill = GetWorld()->SpawnActor<APrimeSkill>(APrimeSkill::StaticClass(),
			AimLoc, GetActorRotation(), param);

		Skill->SetAttack(m_MonsterInfo.Attack);
		Skill->SetOwner(this);
	}
	break;
	}
}

float AMinionPrime::TakeDamage(float DamageAmount,
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

void AMinionPrime::Death()
{
	Super::Death();

	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode)
		GameMode->GetMainHUD()->ChangeBossHPBarType();

	m_Portal->LinkPortal();
}
