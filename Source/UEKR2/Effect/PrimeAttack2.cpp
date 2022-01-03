// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimeAttack2.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
APrimeAttack2::APrimeAttack2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Prime_Helix/Abilities/SpecialAttack2/FX/P_Prime_Helix_SpecialAttack2.P_Prime_Helix_SpecialAttack2'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 2000.f;

	m_Body->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	m_Particle->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
}

// Called when the game starts or when spawned
void APrimeAttack2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APrimeAttack2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrimeAttack2::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	Effect->LoadParticleAsync(TEXT("PrimeNormal"));

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


