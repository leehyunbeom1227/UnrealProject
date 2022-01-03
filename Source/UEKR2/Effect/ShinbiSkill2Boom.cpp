// Fill out your copyright notice in the Description page of Project Settings.


#include "ShinbiSkill2Boom.h"
#include "NormalEffect.h"
#include "../Player/Shinbi.h"

// Sets default values
AShinbiSkill2Boom::AShinbiSkill2Boom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/Primary/FX/P_Mudang_Primary_Impact.P_Mudang_Primary_Impact'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	//m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));

	m_Body->SetWorldScale3D(FVector(2.0f, 2.0f, 1.0f));
}

// Called when the game starts or when spawned
void AShinbiSkill2Boom::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShinbiSkill2Boom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShinbiSkill2Boom::StopEvent(const FHitResult& result)
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


