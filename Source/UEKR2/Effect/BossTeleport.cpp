// Fill out your copyright notice in the Description page of Project Settings.


#include "BossTeleport.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
ABossTeleport::ABossTeleport()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Relentless/FX/P_Sevarog_Relentless_Dustkick.P_Sevarog_Relentless_Dustkick'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Particle->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	m_Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Movement->InitialSpeed = 500.f;
}

// Called when the game starts or when spawned
void ABossTeleport::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossTeleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
