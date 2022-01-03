// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionSiegeWideAttack.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
AMinionSiegeWideAttack::AMinionSiegeWideAttack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_burst.P_ky_burst'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 1000.f;

	m_Body->SetCollisionProfileName(TEXT("EnemyAttack"));
}

// Called when the game starts or when spawned
void AMinionSiegeWideAttack::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinionSiegeWideAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinionSiegeWideAttack::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}

	m_Hit = true;
}
