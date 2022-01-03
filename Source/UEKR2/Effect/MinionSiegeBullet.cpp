// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionSiegeBullet.h"
#include "MinionSiegeWideAttack.h"
#include "NormalEffect.h"
#include "../Monster/Monster.h"

// Sets default values
AMinionSiegeBullet::AMinionSiegeBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//m_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	m_Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	SetRootComponent(m_Body);
	//m_Body->SetupAttachment(m_Root);
	m_Mesh->SetupAttachment(m_Body);
	m_Mesh->SetWorldRotation(FRotator(0.f, 90.f, 0.f));

	m_Particle->SetupAttachment(m_Mesh);
	m_Particle->SetWorldLocation(FVector(0.f, -200.f, 40.f));
	m_Particle->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	m_Body->SetSphereRadius(130.f);
	m_Body->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

	m_Movement->InitialSpeed = 1000.f;
	m_Movement->ProjectileGravityScale = 1.f;

	m_Movement->SetUpdatedComponent(m_Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/Light_Foliage/Meshes/SM_Rock_01.SM_Rock_01'"));

	if (MeshAsset.Succeeded())
		m_Mesh->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>	ParticleAsset(TEXT("ParticleSystem'/Game/AdvancedMagicFX13/Particles/P_ky_stabShot.P_ky_stabShot'"));

	if (ParticleAsset.Succeeded())
		m_Particle->SetTemplate(ParticleAsset.Object);

	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Body->SetCollisionProfileName(TEXT("EnemyAttack"));

	m_DestroyTime = 0.f;
	m_DestroyTimeMax = 5.f;
	m_Distance = 3000.f;
}

// Called when the game starts or when spawned
void AMinionSiegeBullet::BeginPlay()
{
	Super::BeginPlay();

	m_Movement->OnProjectileStop.AddDynamic(this, &AMinionSiegeBullet::ProjectileStop);
}

// Called every frame
void AMinionSiegeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Body->SetWorldRotation(m_Movement->Velocity.Rotation());
	
	m_Movement->Velocity += FVector(0.f, 0.f, -500.f * DeltaTime);

	m_DestroyTime += DeltaTime;

	if (m_DestroyTimeMax <= m_DestroyTime)
		Destroy();
}

void AMinionSiegeBullet::ProjectileStop(const FHitResult& result)
{
	FActorSpawnParameters	Wideparam;
	Wideparam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMinionSiegeWideAttack* WideAttack = GetWorld()->SpawnActor<AMinionSiegeWideAttack>(AMinionSiegeWideAttack::StaticClass(),
		result.ImpactPoint + FVector(0.f, 0.f, 100.f), FRotator(), Wideparam);

	int WideAttackNum = m_Attack / 2;

	WideAttack->SetAttack(WideAttackNum);
	WideAttack->SetOwner(m_Owner);


	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
		result.ImpactPoint, result.Normal.Rotation(), param);

	Effect->LoadParticleAsync(TEXT("MinionNormal"));
	Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));

	m_Mesh->DestroyComponent();
	m_Particle->DestroyComponent();

	if (IsValid(m_Owner))
	{
		FDamageEvent	DmgEvent;

		result.GetActor()->TakeDamage(m_Attack, DmgEvent,
			m_Owner->GetController(), m_Owner);
	}
}
