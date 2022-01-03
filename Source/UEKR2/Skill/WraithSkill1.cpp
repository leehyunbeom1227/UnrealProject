// Fill out your copyright notice in the Description page of Project Settings.


#include "WraithSkill1.h"
#include "../Effect/NormalEffect.h"
#include "../Player/Wraith.h"

AWraithSkill1::AWraithSkill1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_Projectile.P_Wraith_Sniper_Projectile'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 5000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));
}

// Called when the game starts or when spawned
void AWraithSkill1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWraithSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWraithSkill1::StopEvent(const FHitResult& result)
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
