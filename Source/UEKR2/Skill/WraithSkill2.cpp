// Fill out your copyright notice in the Description page of Project Settings.

#include "WraithSkill2.h"
#include "PlayerSkillController.h"
#include "../Player/PlayerCharacter.h"
#include "../Effect/NormalEffect.h"
#include "../Effect/WraithSkill2Fire.h"
#include "../Effect/WraithSkill2Fire.h"
#include "../Player/Wraith.h"

// Sets default values
AWraithSkill2::AWraithSkill2()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIControllerClass = APlayerSkillController::StaticClass();

	m_Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(m_Body);

	m_Particle->SetupAttachment(m_Body);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Drone/FX/P_Wraith_Drone_Attached_TeamColored.P_Wraith_Drone_Attached_TeamColored'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_AttackAccTime = 0.f;
	m_AttackTime = 5.f;
	
	m_Wait = false;
	m_followAccTime = 0.f;
	m_followTime = 3.f;

	m_Movement->ProjectileGravityScale = 0.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));

	m_DestroyTimeMax = 60.f;

	m_AttackEnd = true;
	m_Distance = 1500.f;
}

// Called when the game starts or when spawned
void AWraithSkill2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWraithSkill2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetPlayerInfo().Job != EPlayerJob::Archer)
		Destroy();

	m_followAccTime += DeltaTime;

	if (m_followAccTime >= m_followTime && m_Wait)
	{
		FVector	Loc = m_Owner->GetActorLocation() + FindDirVec(Cast<AActor>(m_Owner), DirVec::RightBack) * 100 + FVector(0.f, 0.f, 100.f);
		FVector Dir = Loc - GetActorLocation();

		m_Movement->Velocity = Dir / 3;

		m_followAccTime = 0.f;
		m_Wait = !m_Wait;
	}
	
	else if (m_followAccTime >= m_followTime)
		m_Wait = !m_Wait;

	if (!m_AttackEnd)
	{
		m_AttackAccTime += DeltaTime;

		if (m_AttackAccTime >= m_AttackTime)
		{
			m_AttackAccTime = 0.f;
			m_AttackEnd = true;
		}
	}

	m_DestroyTime += DeltaTime;

	if (m_DestroyTimeMax <= m_DestroyTime)
		Destroy();
}

// Called to bind functionality to input
void AWraithSkill2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWraithSkill2::Attack(FVector Dir)
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWraithSkill2Fire* Fire = GetWorld()->SpawnActor<AWraithSkill2Fire>(AWraithSkill2Fire::StaticClass(),
		GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), param);

	Fire->SetAttack(m_Attack);
	Fire->SetOwner(m_Owner);
	Fire->SetDir(Dir);
}