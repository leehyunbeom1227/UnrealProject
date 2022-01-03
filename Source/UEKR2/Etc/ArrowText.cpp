// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowText.h"

// Sets default values
AArrowText::AArrowText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_BillBoard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("BillBoard"));
	m_Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));

	SetRootComponent(m_BillBoard);

	m_Text->SetupAttachment(m_BillBoard);

	m_Text->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

	m_Text->SetText(TEXT("Arrow"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MtrlAsset(TEXT("Material'/Game/Etc/Arrow_Mat.Arrow_Mat'"));

	if (MtrlAsset.Succeeded())
	{
		m_BillBoard->AddElement(MtrlAsset.Object, nullptr, false, 30.0f, 30.0f, nullptr);

		//m_BillBoard->SetMaterial(0, MtrlAsset.Object);
	}
}

// Called when the game starts or when spawned
void AArrowText::BeginPlay()
{
	Super::BeginPlay();
	m_Material = m_BillBoard->CreateDynamicMaterialInstance(0);
 
}

// Called every frame
void AArrowText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

