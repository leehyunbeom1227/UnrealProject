// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "../UEKR2GameInstance.h"
#include "../UEKR2GameModeBase.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/Inventory.h"
#include "../UI/MainHUD.h"
#include "../UI/UsePotionWidget.h"
#include "../UI/QuestWidget.h"
#include "../UI/InventoryList.h"
#include "../UI/InventoryTile.h"

// Sets default values
AItemBox::AItemBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Box);

	m_Mesh->SetupAttachment(m_Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/Item/ItemBox.ItemBox'"));

	if (MeshAsset.Succeeded())
		m_Mesh->SetStaticMesh(MeshAsset.Object);

	m_Box->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	m_Box->SetCollisionProfileName(TEXT("PlayerTrigger"));
	m_Box->SetSimulatePhysics(true);

	m_Mesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

	m_Box->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::TriggerBeginOverlap);
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AItemBox::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep
	, const FHitResult& SweepResult)
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		const FUIItemTableInfo* ItemInfo = GameInst->FindUIItemInfo(m_ItemKey);
		
		if (ItemInfo)
		{
			AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

			if (GameMode)
			{
				Inventory* Inven = GameInst->GetInventory();
				
				Inven->AddItem(ItemInfo);

				UInventoryTile* Tile = GameMode->GetMainHUD()->GetInventoryTile();

				if (Tile->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
					Tile->ShowItem();

				UInventoryList* List = GameMode->GetMainHUD()->GetInventoryList();

				if (List->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
					List->ShowItem();

				if (ItemInfo->Key == (int32)EItemKey::HPPotion || ItemInfo->Key == (int32)EItemKey::MPPotion)
				{
					UUsePotionWidget* UsePotionWidget = GameMode->GetMainHUD()->GetUsePotionWidget();
					
					int32 PotionNumber;
					
					if (ItemInfo->Key == UsePotionWidget->GetPotionKey(1))
						PotionNumber = UsePotionWidget->GetPotionNumber(1);
					else if (ItemInfo->Key == UsePotionWidget->GetPotionKey(2))
						PotionNumber = UsePotionWidget->GetPotionNumber(2);
					else
						PotionNumber = -1;

					++PotionNumber;

					if (ItemInfo->Key >= 0)
						UsePotionWidget->SetNumberText(ItemInfo->Key, PotionNumber);
				}
			}
		}
	}

	Destroy();
}