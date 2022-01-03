// Fill out your copyright notice in the Description page of Project Settings.


#include "BossUltimateSkill.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
ABossUltimateSkill::ABossUltimateSkill()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Ultimate/FX/P_UltMeshArc_Expanding.P_UltMeshArc_Expanding'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 2000.f;
}

// Called when the game starts or when spawned
void ABossUltimateSkill::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossUltimateSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossUltimateSkill::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);
	
	Effect->LoadParticleAsync(TEXT("BossNormal"));

	Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));

	m_Particle->DestroyComponent();

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}

	m_Hit = true;
}




