// Fill out your copyright notice in the Description page of Project Settings.


#include "ShinbiSkill2.h"
#include "../Player/Shinbi.h"
#include "../Effect/NormalEffect.h"
#include "../Effect/ShinbiSkill2Boom.h"

// Sets default values
AShinbiSkill2::AShinbiSkill2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_Particle->SetupAttachment(m_Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/Player/Shinbi/SM_Daggers_Stalker1.SM_Daggers_Stalker1'"));
	
	if (MeshAsset.Succeeded())
		m_StaticMesh->SetStaticMesh(MeshAsset.Object);
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>	Asset(TEXT("ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/AttackWolves/FX/P_Attack_Wolves_Trails.P_Attack_Wolves_Trails'"));

	if (Asset.Succeeded())
		m_Particle->SetTemplate(Asset.Object);

	m_Mesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));

	m_Movement->Velocity = FVector(0.f, 0.f, -1.f);

	m_Body->SetCollisionProfileName(TEXT("PlayerAttack"));
}

// Called when the game starts or when spawned
void AShinbiSkill2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShinbiSkill2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShinbiSkill2::StopEvent(const FHitResult& result)
{
	Super::StopEvent(result);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.ImpactNormal.Rotation(), param);

	AShinbiSkill2Boom* Boom = GetWorld()->SpawnActor<AShinbiSkill2Boom>(AShinbiSkill2Boom::StaticClass(),
		result.ImpactPoint + FVector(0.f, 0.f, 100.f), result.ImpactNormal.Rotation(), param);

	int BoomAttack = m_Attack / 2;
	Boom->SetAttack(BoomAttack);
	Boom->SetOwner(m_Owner);

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		m_Owner->LoadParticleEffect(Effect);

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}

	Effect->LoadSoundAsync(TEXT("HitFire"));

	m_Particle->DestroyComponent();
	m_StaticMesh->DestroyComponent();

	m_DestroyEnable = true;
}

