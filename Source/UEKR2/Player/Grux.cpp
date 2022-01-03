// Fill out your copyright notice in the Description page of Project Settings.


#include "Grux.h"
#include "PlayerAnim.h"
#include "GameFramework/PlayerController.h"
#include "../UEKR2GameInstance.h"
#include "../Skill/GruxSkill1.h"
#include "../Skill/GruxSkill2.h"
#include "../Effect/NormalEffect.h"


AGrux::AGrux()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	GrexAsset(TEXT("SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Skins/Tier_1/War_Chief/Meshes/GruxWarchief.GruxWarchief'"));

	if (GrexAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GrexAsset.Object);
		m_PlayerMesh = GrexAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>	GrexAnimAsset(TEXT("AnimBlueprint'/Game/Player/Grux/BPGruxAnim.BPGruxAnim_C'"));

	if (GrexAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(GrexAnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/Player/Grux/AMGruxAttack1.AMGruxAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/Grux/AMGruxAttack2.AMGruxAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack3Asset(TEXT("AnimMontage'/Game/Player/Grux/AMGruxAttack3.AMGruxAttack3'"));

	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack4Asset(TEXT("AnimMontage'/Game/Player/Grux/AMGruxAttack4.AMGruxAttack4'"));

	if (Attack4Asset.Succeeded())
		m_AttackMontageArray.Add(Attack4Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/Grux/AMGruxSkill1.AMGruxSkill1'"));

	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Grux/AMGruxSkill2.AMGruxSkill2'"));

	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DashAsset(TEXT("AnimMontage'/Game/Player/Grux/AMGruxDash.AMGruxDash'"));

	if (DashAsset.Succeeded())
		m_DashMontage = DashAsset.Object;

	m_AttackIndex = 0;

	m_PlayerColor = FVector(3000.f, 200.f, 200.f);
}

// Called when the game starts or when spawned
void AGrux::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AGrux::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGrux::AttackEnd()
{
	Super::AttackEnd();

	m_AttackIndex = 0;
}

void AGrux::UseSkill(int32 Index)
{
	switch (Index)
	{
	case 0:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGruxSkill1* Skill = GetWorld()->SpawnActor<AGruxSkill1>(AGruxSkill1::StaticClass(),
			GetActorLocation() + GetActorForwardVector() * 100.f,
			GetActorRotation(), param);

		Skill->SetAttack(m_PlayerInfo.Attack * m_Skill1Power * m_AttackBuff);
		Skill->SetOwner(this);

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonGrux/Characters/Heroes/Grux/Sounds/SoundWaves/Grux_Effort_Attack_03.Grux_Effort_Attack_03'"));

	}
	break;

	case 1:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGruxSkill2* Skill = GetWorld()->SpawnActor<AGruxSkill2>(AGruxSkill2::StaticClass(),
			GetActorLocation(),	GetActorRotation(), param);

		Skill->SetOwner(this);

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonGrux/Characters/Heroes/Grux/Sounds/SoundWaves/Grux_Effort_Ability_Ultimate_03.Grux_Effort_Ability_Ultimate_03'"));

		m_BoolAttackBuff = true;
		m_AttackBuff = m_Skill1Power;
	}
	break;
	}
}