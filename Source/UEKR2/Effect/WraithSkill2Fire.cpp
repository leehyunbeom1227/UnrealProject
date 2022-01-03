// Fill out your copyright notice in the Description page of Project Settings.


#include "WraithSkill2Fire.h"
#include "NormalEffect.h"
#include "../Player/Wraith.h"

AWraithSkill2Fire::AWraithSkill2Fire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_Trail.P_Wraith_Primary_Trail'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));
}

// Called when the game starts or when spawned
void AWraithSkill2Fire::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWraithSkill2Fire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Movement->Velocity = m_Dir * 2000;
}

void AWraithSkill2Fire::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		m_Owner->LoadParticleEffect(Effect);

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}

	Effect->LoadSoundAsync(TEXT("HitNormal"));

	m_Hit = true;
}
