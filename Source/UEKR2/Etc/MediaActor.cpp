// Fill out your copyright notice in the Description page of Project Settings.


#include "MediaActor.h"

// Sets default values
AMediaActor::AMediaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_MediaSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSound"));

	SetRootComponent(m_Mesh);

	m_MediaSound->SetupAttachment(m_Mesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Movie/MediaPlane.MediaPlane'"));

	if (MeshAsset.Succeeded())
		m_Mesh->SetStaticMesh(MeshAsset.Object);

	m_Mesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
	m_Mesh->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));

	static ConstructorHelpers::FObjectFinder<UMediaPlayer> MediaAsset(TEXT("MediaPlayer'/Game/Movie/IntroMedia.IntroMedia'"));

	if (MediaAsset.Succeeded())
		m_MediaAsset = MediaAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMediaTexture> MediaTexAsset(TEXT("MediaTexture'/Game/Movie/IntroMediaTexture.IntroMediaTexture'"));

	if (MediaTexAsset.Succeeded())
		m_MediaTexture = MediaTexAsset.Object;
	
	static ConstructorHelpers::FObjectFinder<UMediaSource> MediaSourceAsset(TEXT("FileMediaSource'/Game/Movie/SampleVideo.SampleVideo'"));

	if (MediaSourceAsset.Succeeded())
		m_MediaSource = MediaSourceAsset.Object;
}

// Called when the game starts or when spawned
void AMediaActor::BeginPlay()
{
	Super::BeginPlay();
	
	m_MediaAsset->OpenSource(m_MediaSource);

	m_Material = m_Mesh->CreateDynamicMaterialInstance(0);

	m_Material->SetTextureParameterValue(TEXT("Texture"), m_MediaTexture);

	m_MediaSound->SetMediaPlayer(m_MediaAsset);

	m_MediaAsset->Play();

}

// Called every frame
void AMediaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

