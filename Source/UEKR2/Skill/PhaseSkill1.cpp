// Fill out your copyright notice in the Description page of Project Settings.


#include "PhaseSkill1.h"
#include "../Effect/NormalEffect.h"
#include "../Player/Phase.h"

// Sets default values
APhaseSkill1::APhaseSkill1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonPhase/FX/Particles/Abilities/Flash/FX/P_PhaseFlash.P_PhaseFlash'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);
	
	//m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));
	m_Body->SetWorldScale3D(FVector(10.0f, 10.0f, 1.0f));
	m_DestroyTimeMax = 1.f;
}

// Called when the game starts or when spawned
void APhaseSkill1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APhaseSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APhaseSkill1::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}

	Effect->LoadParticle(TEXT("ParticleSystem'/Game/ParagonPhase/FX/Particles/Abilities/Link/FX/P_PhaseLinkImpact.P_PhaseLinkImpact'"));
	Effect->LoadSoundAsync(TEXT("HitNormal"));

	m_Hit = true;
}
