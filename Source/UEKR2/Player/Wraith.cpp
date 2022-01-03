// Fill out your copyright notice in the Description page of Project Settings.


#include "Wraith.h"
#include "PlayerAnim.h"
#include "../Effect/NormalEffect.h"
#include "../Skill/WraithAttack.h"
#include "../Skill/WraithSkill1.h"
#include "../Skill/WraithSkill2.h"
#include "Weapon.h"

AWraith::AWraith()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	WraithAsset(TEXT("SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'"));

	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
	if (WraithAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(WraithAsset.Object);
		m_PlayerMesh = WraithAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>	WraithAnimAsset(TEXT("AnimBlueprint'/Game/Player/Wraith/BPWraithAnim.BPWraithAnim_C'"));

	if (WraithAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(WraithAnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/Player/Wraith/AMWraithAttack1.AMWraithAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/Wraith/AMWraithSkill1.AMWraithSkill1'"));

	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);
	
	//static ConstructorHelpers::FClassFinder<AActor>	Skill1Class(TEXT("Blueprint'/Game/Player/Greystone/BPGreystoneSkill1.BPGreystoneSkill1_C'"));

	//if (Skill1Class.Succeeded())
	//	m_Skiill1Class = Skill1Class.Class;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Wraith/AMWraithSkill2.AMWraithSkill2'"));

	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DashAsset(TEXT("AnimMontage'/Game/Player/Wraith/AMWraithDash.AMWraithDash'"));

	if (DashAsset.Succeeded())
		m_DashMontage = DashAsset.Object;

	m_AttackIndex = 0;

	m_Attack = false;

	m_PlayerColor = FVector(200.f, 200.f, 200.f);
}

// Called when the game starts or when spawned
void AWraith::BeginPlay()
{
	// Super : 부모클래스를 의미한다.
	Super::BeginPlay();

	//FActorSpawnParameters	param;
	//param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//m_Weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(),
	//	FVector::ZeroVector, FRotator::ZeroRotator,
	//	param);

	//m_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	//	TEXT("BackWeapon"));

	////m_Weapon->DetachFromActor

	//m_Weapon->SetMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Weapon_ForgingHammers/SK_Forging_Mallet_03.SK_Forging_Mallet_03'"));
}

// Called every frame
void AWraith::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWraith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWraith::NormalAttack()
{
	Super::NormalAttack();

	if (m_Attack == true)
		return;

	m_Attack = true;

	FVector	PlayerLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FVector	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWraithAttack* Bullet = GetWorld()->SpawnActor<AWraithAttack>(
		AWraithAttack::StaticClass(), MuzzleLoc, GetActorRotation(),
		param);

	Bullet->SetAttack(m_PlayerInfo.Attack * m_AttackBuff);
	Bullet->SetOwner(this);
}

void AWraith::AttackEnd()
{
	Super::AttackEnd();

	m_Attack = false;

	m_AttackIndex = 0;
}

void AWraith::UseSkill(int32 Index)
{
	switch (Index)
	{
	case 0:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

		AWraithSkill1* Skill = GetWorld()->SpawnActor<AWraithSkill1>(AWraithSkill1::StaticClass(),
			MuzzleLoc, GetActorRotation(), param);

		Skill->SetAttack(m_PlayerInfo.Attack * m_Skill1Power * m_AttackBuff);
		Skill->SetOwner(this);

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonWraith/Audio/Wavs/Wraith_Effort_Swing_010.Wraith_Effort_Swing_010'"));
	
	}
	break;

	case 1:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Dir = FindDirVec(Cast<AActor>(this), 3);

		AWraithSkill2* Skill = GetWorld()->SpawnActor<AWraithSkill2>(AWraithSkill2::StaticClass(),
			GetActorLocation() + Dir * 100.f, GetActorRotation(), param);

		Skill->SetAttack(m_PlayerInfo.Attack * m_Skill1Power * m_AttackBuff);
		Skill->SetOwner(this);

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonWraith/Audio/Wavs/Wraith_Effort_Ability_Q_010.Wraith_Effort_Ability_Q_010'"));

	}
	break;
	}
}
