// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase.h"
#include "PlayerAnim.h"
#include "../Effect/NormalEffect.h"
#include "../Skill/PhaseAttack.h"
#include "../Skill/PhaseSkill1.h"
#include "../Skill/PhaseSkill2.h"

APhase::APhase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	PhaseAsset(TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Skins/Tier_1_5/Kitty/Meshes/Phase_Kitty_GDC.Phase_Kitty_GDC'"));

	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
	if (PhaseAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PhaseAsset.Object);
		m_PlayerMesh = PhaseAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>	PhaseAnimAsset(TEXT("AnimBlueprint'/Game/Player/Phase/BPPhaseAnim.BPPhaseAnim_C'"));

	if (PhaseAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(PhaseAnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseAttak1.AMPhaseAttak1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseAttak2.AMPhaseAttak2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack3Asset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseAttak3.AMPhaseAttak3'"));

	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack4Asset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseAttak4.AMPhaseAttak4'"));

	if (Attack4Asset.Succeeded())
		m_AttackMontageArray.Add(Attack4Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseSkill1.AMPhaseSkill1'"));

	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseSkill2.AMPhaseSkill2'"));

	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2FireAsset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseSkill2Fire.AMPhaseSkill2Fire'"));

	if (Skill2FireAsset.Succeeded())
		m_Skill2FireMontage = Skill2FireAsset.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DashAsset(TEXT("AnimMontage'/Game/Player/Phase/AMPhaseDash.AMPhaseDash'"));

	if (DashAsset.Succeeded())
		m_DashMontage = DashAsset.Object;

	m_AttackIndex = 0;
	m_SkillCharging = false;
	m_SkillFire = false;

	m_PlayerColor = FVector(200.f, 200.f, 3000.f);
}

// Called when the game starts or when spawned
void APhase::BeginPlay()
{
	// Super : 부모클래스를 의미한다.
	Super::BeginPlay();

}

// Called every frame
void APhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APhase::NormalAttack()
{
	FVector	Forward = GetActorForwardVector();

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APhaseAttack* Bullet = GetWorld()->SpawnActor<APhaseAttack>(
		APhaseAttack::StaticClass(), GetActorLocation() + 100 * Forward, GetActorRotation(),
		param);

	Bullet->SetAttack(m_PlayerInfo.Attack * m_AttackBuff);
	Bullet->SetOwner(this);
}

// Called to bind functionality to input
void APhase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APhase::Attack()
{
	if (m_SkillCharging)
		Skill2Fire();

	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
	else if (!m_AnimInstance->Montage_IsPlaying(m_AttackMontageArray[m_AttackIndex]))
	{
		m_AnimInstance->Montage_SetPosition(m_AttackMontageArray[m_AttackIndex], 0.f);
		m_AnimInstance->Montage_Play(m_AttackMontageArray[m_AttackIndex]);
		m_AnimInstance->SetAttackEnable(true);

		m_AttackIndex = (m_AttackIndex + 1) % m_AttackMontageArray.Num();
	}
}

void APhase::Skill2()
{
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
	if (!m_AnimInstance->Montage_IsPlaying(m_SkillMontageArray[1]))
	{
		m_AnimInstance->Montage_SetPosition(m_SkillMontageArray[1], 0.f);
		m_AnimInstance->Montage_Play(m_SkillMontageArray[1]);
	}

	m_SkillFire = false;
}

void APhase::Skill2Fire()
{
	// 0번 공격 모션이 재생이 안되고 있다면 재생을 시켜준다.
	if (!m_AnimInstance->Montage_IsPlaying(m_Skill2FireMontage))
	{
		m_AnimInstance->Montage_SetPosition(m_Skill2FireMontage, 0.f);
		m_AnimInstance->Montage_Play(m_Skill2FireMontage);
	}
}

void APhase::AttackEnd()
{
	Super::AttackEnd();
	
	m_AttackIndex = 0;
}

void APhase::UseSkill(int32 Index)
{
	switch (Index)
	{
	case 0:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APhaseSkill1* Skill = GetWorld()->SpawnActor<APhaseSkill1>(APhaseSkill1::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Skill->SetAttack(m_PlayerInfo.Attack * m_Skill1Power * m_AttackBuff);
		Skill->SetOwner(this);

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonPhase/Characters/Heroes/Phase/Sounds/SoundWaves/Phase_Effort_Ability_Q_02.Phase_Effort_Ability_Q_02'"));
	}
	break;

	case 1:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int i = 0; i < 8; ++i)
		{
			FVector Dir = FindDirVec(Cast<AActor>(this), i);
			APhaseSkill2* Skill = GetWorld()->SpawnActor<APhaseSkill2>(APhaseSkill2::StaticClass(),
				GetActorLocation() + Dir * 100,
				GetActorRotation(), param);

			Skill->SetDir(Dir);
			Skill->SetAttack(m_PlayerInfo.Attack * m_Skill2Power * m_AttackBuff);
			Skill->SetOwner(this);
			Skill->SetNumber(i);

			m_SkillCharging = true;
		}

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonPhase/Characters/Heroes/Phase/Sounds/SoundWaves/Phase_Effort_Attack_01.Phase_Effort_Attack_01'"));
	}
	break;
	}
}

void APhase::UseSkillFire(int32 Index)
{
	switch (Index)
	{
	case 0:
		break;
	case 1:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonPhase/Characters/Heroes/Phase/Sounds/SoundWaves/Phase_Effort_Swing_03.Phase_Effort_Swing_03'"));
		
		m_SkillCharging = false;
		m_SkillFire = true;
	}
	break;
	}
}
