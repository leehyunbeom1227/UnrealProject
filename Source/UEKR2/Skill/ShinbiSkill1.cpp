// Fill out your copyright notice in the Description page of Project Settings.


#include "ShinbiSkill1.h"
#include "../Effect/NormalEffect.h"
#include "../Player/Shinbi.h"

// Sets default values
AShinbiSkill1::AShinbiSkill1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SecondParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SecondParticle"));
	m_TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonShinbi/FX/Particles/Shinbi_Wolf/FX/P_Attack_Wolves_Body.P_Attack_Wolves_Body'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	SecondParticleAsset(TEXT("ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/Dash/FX/P_Mudang_Dash_Base.P_Mudang_Dash_Base'"));

	if (SecondParticleAsset.Succeeded())				
		m_SecondParticle->SetTemplate(SecondParticleAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	TrailAsset(TEXT("ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/AttackWolves/FX/P_Attack_Wolves_Trails.P_Attack_Wolves_Trails'"));

	if (TrailAsset.Succeeded())
		m_TrailParticle->SetTemplate(TrailAsset.Object);

	m_SecondParticle->SetupAttachment(m_Particle);
	m_TrailParticle->SetupAttachment(m_SecondParticle);

	m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));
	m_DestroyTimeMax = 1.f;
}

// Called when the game starts or when spawned
void AShinbiSkill1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShinbiSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Movement->Velocity = m_Dir * 1000;
}

void AShinbiSkill1::StopEvent(const FHitResult& result)
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
