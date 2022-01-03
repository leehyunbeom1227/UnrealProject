// Fill out your copyright notice in the Description page of Project Settings.

//(X=-300.000000,Y=3980.000000,Z=8020.000000)
#include "PortalOpenTrigger.h"
#include "PortalTrigger.h"

// Sets default values
APortalOpenTrigger::APortalOpenTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_hit_shine.P_ky_hit_shine'"));

	if (ParticleAsset.Succeeded())
		m_ParticleSystem->SetTemplate(ParticleAsset.Object);

	SetActorScale3D(FVector(10.f, 10.f, 10.f));

	m_ParticleSystem->SetRelativeScale3D(FVector(0.1, 0.1f, 0.1f));
	m_ParticleSystem->SetWorldLocation(FVector(0.f, 0.f, 0.f));

	m_SequenceAsset = nullptr;
	m_SequencePlayer = nullptr;
}

// Called when the game starts or when spawned
void APortalOpenTrigger::BeginPlay()
{
	Super::BeginPlay();

	m_ParticleSystem->Activate(true);

	m_TriggerBeginDelegate.AddDynamic(this, &APortalOpenTrigger::TriggerBegin);
}

// Called every frame
void APortalOpenTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalOpenTrigger::TriggerBegin()
{
	m_ParticleSystem->SetVisibility(false);

	m_Portal->LinkPortal();

	if (m_SequenceAsset)
	{
		if (!m_SequencePlayer)
		{
			m_SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld(), m_SequenceAsset, FMovieSceneSequencePlaybackSettings(), m_SequenceActor);
		}

		m_SequencePlayer->Play();
	}
}
