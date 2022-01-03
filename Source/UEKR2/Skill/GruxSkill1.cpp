// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxSkill1.h"
#include "../Effect/NormalEffect.h"
#include "../Player/Grux.h"

// Sets default values
AGruxSkill1::AGruxSkill1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonGrux/FX/Particles/Skins/Grux_Beetle_Magma/P_Grux_Magma_Ultimate_Cast2.P_Grux_Magma_Ultimate_Cast2'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	//m_Movement->InitialSpeed = 2000.f;
	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));

	m_DestroyTimeMax = 1.f;
}

// Called when the game starts or when spawned
void AGruxSkill1::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGruxSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGruxSkill1::StopEvent(const FHitResult& result)
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

	m_Particle->DestroyComponent();

	m_Hit = true;
}
