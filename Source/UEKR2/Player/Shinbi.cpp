// Fill out your copyright notice in the Description page of Project Settings.


#include "Shinbi.h"
#include "PlayerAnim.h"
#include "../Effect/NormalEffect.h"
#include "../Skill/ShinbiSkill1.h"
#include "../Skill/ShinbiSkill2.h"
#include "GameFramework/PlayerController.h"

AShinbi::AShinbi()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	ShinbiAsset(TEXT("SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'"));

	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
	if (ShinbiAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(ShinbiAsset.Object);
		m_PlayerMesh = ShinbiAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>	ShinbiAnimAsset(TEXT("AnimBlueprint'/Game/Player/Shinbi/BPShinbiAnim.BPShinbiAnim_C'"));

	if (ShinbiAnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(ShinbiAnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack1Asset(TEXT("AnimMontage'/Game/Player/Shinbi/AMShinbiAttack1.AMShinbiAttack1'"));

	if (Attack1Asset.Succeeded())
		m_AttackMontageArray.Add(Attack1Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack2Asset(TEXT("AnimMontage'/Game/Player/Shinbi/AMShinbiAttack2.AMShinbiAttack2'"));

	if (Attack2Asset.Succeeded())
		m_AttackMontageArray.Add(Attack2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Attack3Asset(TEXT("AnimMontage'/Game/Player/Shinbi/AMShinbiAttack3.AMShinbiAttack3'"));

	if (Attack3Asset.Succeeded())
		m_AttackMontageArray.Add(Attack3Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill1Asset(TEXT("AnimMontage'/Game/Player/Shinbi/AMShinbiSkill1.AMShinbiSkill1'"));

	if (Skill1Asset.Succeeded())
		m_SkillMontageArray.Add(Skill1Asset.Object);
;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Skill2Asset(TEXT("AnimMontage'/Game/Player/Shinbi/AMShinbiSkill2.AMShinbiSkill2'"));

	if (Skill2Asset.Succeeded())
		m_SkillMontageArray.Add(Skill2Asset.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	DashAsset(TEXT("AnimMontage'/Game/Player/Shinbi/AMshinbiDash.AMshinbiDash'"));

	if (DashAsset.Succeeded())
		m_DashMontage = DashAsset.Object;

	m_AttackIndex = 0;

	m_PlayerColor = FVector(3000.f, 2000.f, 2500.f);

}

// Called when the game starts or when spawned
void AShinbi::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShinbi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShinbi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShinbi::AttackEnd()
{
	Super::AttackEnd();

	m_AttackIndex = 0;
}

void AShinbi::UseSkill(int32 Index)
{
	switch (Index)
	{
	case 0:
	{
		FHitResult	result;

		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, false, result);
		
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Loc = result.Location;
		FVector Dir = result.Location - GetActorLocation();
		FVector DirNor = FVector(Dir.X / Dir.Size(), Dir.Y / Dir.Size(), Dir.Z / Dir.Size());

		AShinbiSkill1* Skill = GetWorld()->SpawnActor<AShinbiSkill1>(AShinbiSkill1::StaticClass(),
			GetActorLocation() + GetActorForwardVector() * 100,	GetActorRotation(), param);
		
		Skill->SetAttack(m_PlayerInfo.Attack * m_Skill1Power * m_AttackBuff);
		Skill->SetOwner(this);
		Skill->SetDir(DirNor);

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Attack_01.Shinbi_Effort_Attack_01'"));
	}
	break;

	case 1:
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int i = 1; i < 8; i += 2)
		{
			FVector Dir = FindDirVec(Cast<AActor>(this), i);
			AShinbiSkill2* Skill = GetWorld()->SpawnActor<AShinbiSkill2>(AShinbiSkill2::StaticClass(),
				GetActorLocation() + Dir * 500.f + FVector(0.f, 0.f, 300.f), GetActorRotation(), param);

			Skill->SetAttack(m_PlayerInfo.Attack * m_Skill2Power * m_AttackBuff);
			Skill->SetOwner(this);
		}

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			GetActorLocation(), GetActorRotation(), param);

		Effect->LoadSound(TEXT("SoundWave'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Sounds/SoundWaves/Shinbi_Effort_Ability_E_03.Shinbi_Effort_Ability_E_03'"));

	}
	break;
	}
}
