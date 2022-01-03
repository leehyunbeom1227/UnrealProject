// Fill out your copyright notice in the Description page of Project Settings.


#include "WraithAttack.h"
#include "../Effect/NormalEffect.h"
#include "../Player/Wraith.h"

// Sets default values
AWraithAttack::AWraithAttack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_Trail.P_Wraith_Primary_Trail'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));
	m_DestroyTimeMax = 1.f;

	m_Body->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

// Called when the game starts or when spawned
void AWraithAttack::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWraithAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWraithAttack::StopEvent(const FHitResult& result)
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
