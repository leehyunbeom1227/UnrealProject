// Fill out your copyright notice in the Description page of Project Settings.


#include "PhaseSkill2.h"
#include "../Effect/NormalEffect.h"
#include "../Player/Phase.h"

// Sets default values
APhaseSkill2::APhaseSkill2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonPhase/FX/Particles/Abilities/Ultimate/FX/P_PhaseUlt.P_PhaseUlt'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	//m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));
}

// Called when the game starts or when spawned
void APhaseSkill2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APhaseSkill2::Tick(float DeltaTime)
{
	m_DestroyTime += DeltaTime;

	if (m_DestroyTimeMax <= m_DestroyTime)
	{
		Destroy();
		m_Owner->FireEnd();
	}

	if (m_Hit)
		Destroy();


	if (m_Owner)
	{
		if (!m_Owner->GetSkillFire())
		{
			FVector Dir = FindDirVec(Cast<AActor>(m_Owner), m_Number);
			m_Body->SetWorldLocation(m_Owner->GetActorLocation() + Dir * 100);
			
			m_DirVector = Dir;
		}

		else
			m_Movement->Velocity = m_DirVector * 1000;
	}
}

void APhaseSkill2::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	Effect->LoadSoundAsync(TEXT("HitNormal"));

	m_Particle->DestroyComponent();

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		m_Owner->LoadParticleEffect(Effect);

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}

	m_Hit = true;
}
