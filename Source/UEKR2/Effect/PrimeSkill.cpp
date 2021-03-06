// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimeSkill.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
APrimeSkill::APrimeSkill()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Prime_Helix/Abilities/PrimaryAttack/FX/P_PrimeHelix_Attack.P_PrimeHelix_Attack'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 2000.f;

	m_Body->SetCollisionProfileName(TEXT("EnemyAttack"));

}

// Called when the game starts or when spawned
void APrimeSkill::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APrimeSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrimeSkill::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	Effect->LoadParticleAsync(TEXT("PrimeNormal"));
	// Sound
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


