// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalTrigger.h"
#include "../UEKR2GameInstance.h"

// Sets default values
APortalTrigger::APortalTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Material = CreateDefaultSubobject<UMaterialInstanceDynamic>(TEXT("Material"));
	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance>	MtrlAsset(TEXT("MaterialInstanceConstant'/Game/Etc/Portal_Inst.Portal_Inst'"));

	if (MtrlAsset.Succeeded())
		m_Material = MtrlAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/Etc/DefalutBox.DefalutBox'"));

	if (MeshAsset.Succeeded())
		m_Mesh->SetStaticMesh(MeshAsset.Object);

	m_Trigger->SetRelativeScale3D(FVector(3.f, 3.f, 5.f));

	m_Mesh->SetMaterial(0, m_Material);
	m_Mesh->SetVisibility(false);
	m_ParticleSystem->SetVisibility(false);
	m_ParticleSystem->SetRelativeScale3D(FVector(0.1, 0.1f, 0.1f));
	m_ParticleSystem->SetWorldLocation(FVector(0.f, 0.f, 0.f));

	m_PortalSize = false;
}

// Called when the game starts or when spawned
void APortalTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	m_ParticleSystem->EmitterDelay = 0;
}

// Called every frame
void APortalTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_PortalSize)
	{
		FVector ReScale3D = m_Mesh->GetRelativeScale3D();

		m_Mesh->SetRelativeScale3D(ReScale3D + FVector(0.75f * DeltaTime, 0.f, 0.45f * DeltaTime));

		if (m_Mesh->GetRelativeScale3D().X >= 1.5f && m_Mesh->GetRelativeScale3D().Z >= 0.9f)
			m_PortalSize = false;
	}

	m_Mesh->AddLocalRotation(FRotator(180.f * DeltaTime, 0.f, 0.f));
}

void APortalTrigger::LinkPortal()
{
	m_Mesh->SetVisibility(true);

	m_PortalSize = true;
	m_ParticleSystem->SetVisibility(true);
	m_TriggerBeginDelegate.AddDynamic(this, &APortalTrigger::TriggerBegin);
}

void APortalTrigger::TriggerBegin()
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	GameInst->SavePlayerInfo();

	FName fName = FName(*m_NextLevelName);
	UGameplayStatics::OpenLevel(GetWorld(), fName);
}

