// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxSkill2.h"
#include "../Player/Grux.h"

AGruxSkill2::AGruxSkill2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonGrux/FX/Particles/Skins/Grux_Beetle_Magma/P_Grux_Magma_StampedTrail.P_Grux_Magma_StampedTrail'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AGruxSkill2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGruxSkill2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Owner)
		m_Particle->SetWorldLocation(m_Owner->GetActorLocation());
}


