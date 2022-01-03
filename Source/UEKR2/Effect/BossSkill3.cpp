// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSkill3.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
ABossSkill3::ABossSkill3()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/SoulStackPassive/FX/P_ShadowTrailsCharSelect.P_ShadowTrailsCharSelect'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Tick = true;

	m_DestroyTime = 10.f;
}

// Called when the game starts or when spawned
void ABossSkill3::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABossSkill3::TickDagame, 1.0f, true, 2.0f);
}

// Called every frame
void ABossSkill3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossSkill3::StopEvent(const FHitResult& result)
{
	if (m_Tick)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.Normal.Rotation(), param);

		Effect->LoadParticleAsync(TEXT("BossNormal"));

		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));

		if (IsValid(m_Owner))
		{
			FDamageEvent	DmgEvent;

			result.GetActor()->TakeDamage(m_Attack, DmgEvent,
				m_Owner->GetController(), m_Owner);
		}

		m_Tick = false;
		m_Hit = true;
	}
}

void ABossSkill3::TickDagame()
{
	m_Tick = true;
}
