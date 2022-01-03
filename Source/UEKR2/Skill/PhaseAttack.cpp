// Fill out your copyright notice in the Description page of Project Settings.


#include "PhaseAttack.h"
#include "../Effect/NormalEffect.h"
#include "../Player/Phase.h"


// Sets default values
APhaseAttack::APhaseAttack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonPhase/FX/Particles/Abilities/Beam/FX/P_PhaseBeam.P_PhaseBeam'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	//m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));

	m_DestroyTimeMax = 1.f;
}

// Called when the game starts or when spawned
void APhaseAttack::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APhaseAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhaseAttack::StopEvent(const FHitResult& result)
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
