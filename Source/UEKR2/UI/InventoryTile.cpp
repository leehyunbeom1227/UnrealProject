// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTile.h"
#include "InventoryTileData.h"
#include "../UEKR2GameInstance.h"
#include "../UEKR2GameModeBase.h"
#include "MainHUD.h"
#include "Components/CanvasPanelSlot.h"
#include "../Player/Inventory.h"
#include "../Player/PlayerCharacter.h"


// Tile 사이즈가 0일 경우 터짐

void UInventoryTile::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemCount = 0;

	m_InventoryTile = Cast<UTileView>(GetWidgetFromName(TEXT("InventoryTile")));
	m_InventorySlot = Cast<UTileView>(GetWidgetFromName(TEXT("InventorySlot")));
	m_ItemDescWidget = Cast<UItemDescWidget>(GetWidgetFromName(TEXT("UI_ItemDesc")));
	m_GoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldText")));
	m_ChangeButton = Cast<UButton>(GetWidgetFromName(TEXT("ChangeButton")));

	//if (m_InvenInitDelegate.IsBound())
	//	m_InvenInitDelegate.Execute();

	m_InventoryTile->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	m_InventorySlot->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	/*
	FString ItemNameArray[3] =
	{
		TEXT("BF대검"),
		TEXT("칠흑의양날도끼"),
		TEXT("HP포션")
	};

	// 슬롯은 무조건 20개
	for (int32 i = 0; i < 20; ++i)
	{
		UInventoryTileData* Data = NewObject<UInventoryTileData>(this, UInventoryTileData::StaticClass());

		m_InventorySlot->AddItem(Data);
		m_SlotArray.Add(Data);
	}

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	//UImage* Item0 = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Item0"));

	//UCanvasPanel* Canvas = WidgetTree->FindWidget<UCanvasPanel>(TEXT("CanvasPanel1"));

	//UCanvasPanelSlot* PanelSlot = Canvas->AddChildToCanvas(Item0);

	//PanelSlot->SetPosition(FVector2D(200.f, 50.f));
	//PanelSlot->SetSize(FVector2D(50.f, 50.f));

	for (int32 i = 0; i < 100; ++i)
	{
		int32 Index = FMath::RandRange(0, 2);

		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(ItemNameArray[Index]);

		UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,
			UInventoryItemDataTile::StaticClass());

		Data->SetNameText(Info->Name);		
		Data->SetIconTexture(Info->IconTexture);
		Data->SetIndex(i);

		m_InventoryTile->AddItem(Data);

		if (i >= 20)
		{
			UInventoryTileData* Data1 = NewObject<UInventoryTileData>(this, UInventoryTileData::StaticClass());

			m_InventorySlot->AddItem(Data1);
	
			m_SlotArray.Add(Data1);
		}
	}
	
	m_ItemCount = 100;
	*/

	m_ItemCount = 100;
	m_MouseHovered = false;

	m_ChangeButton->OnClicked.AddDynamic(this, &UInventoryTile::ChangeSlotType);
	m_InventoryTile->OnItemDoubleClicked().AddUObject(this, &UInventoryTile::ItemDoubleClick);
	//m_InventoryTile->OnItemClicked().AddUObject(this, &UInventoryTile::ItemClick);
	m_InventoryTile->OnItemIsHoveredChanged().AddUObject(this, &UInventoryTile::ItemHOvered);
	//m_InventoryTile->OnItemScrolledIntoView().AddUObject(this, &UInventoryTile::ItemScroll);
}

void UInventoryTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//if (m_MouseHovered)
	//{

	//	//m_ItemDescWidget->SetPositionInViewport(FVector2D(MouseX + 1, MouseY + 1));
	//	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemDescWidget->Slot);

	//	if (PanelSlot)
	//	{
	//		float MouseX = 0.0f, MouseY = 0.0f;
	//		if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MouseX, MouseY))
	//		{
	//			PrintViewport(1.0f, FColor::Red, FString::Printf(TEXT("X : % .5f, Y : %.5f"), MouseX, MouseY));
	//		}

	//		PanelSlot->SetPosition(FVector2D(MouseX, MouseY));

	//	}
	//}
}

FReply UInventoryTile::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (m_MouseHovered)
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemDescWidget->Slot);

		if (PanelSlot)
		{
			FVector2D WidgetPos = InMouseEvent.GetScreenSpacePosition();

			//USlateBlueprintLibrary::ScreenToWidgetLocal(this, InGeometry, InMouseEvent.GetScreenSpacePosition(), WidgetPos);

			WidgetPos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, WidgetPos);

			PanelSlot->SetPosition(FVector2D(WidgetPos.X + 1, WidgetPos.Y + 1));
			PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("X : %.5f Y : %.5f"), WidgetPos.X, WidgetPos.Y));
		}
	}

	return FReply::Handled();
}

void UInventoryTile::ItemDoubleClick(UObject* Data)
{
	UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Data);

	if (Item)
	{
		UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

		if (GameInst)
		{
			Inventory* Inven = GameInst->GetInventory();

			if (Item->GetKey() != -1)
			{
				FInventoryItem* InvenItem = Inven->GetInvenItemByKey(Item->GetKey());

				int32 InvenItemNumber = InvenItem->Number;

				APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

				Inven->UseItem(Player, InvenItem);

				AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

				if (IsValid(GameMode))
					GameMode->GetMainHUD()->GetUsePotionWidget()->SetNumberText(Item->GetKey(), InvenItemNumber - 1);

				ShowItem();
			}
		}
	}
}

//void UInventoryTile::ItemScroll(UObject* Data, UUserWidget& widget)
//{
//
//}

void UInventoryTile::ItemHOvered(UObject* Data, bool Hovered)
{
	m_MouseHovered = Hovered;
	 
	if (Hovered)
	{
		m_ItemDescWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UInventoryItemDataTile* Item = Cast<UInventoryItemDataTile>(Data);
		if (Item)
		{
			m_ItemDescWidget->SetNameText(Item->GetNameText());
			m_ItemDescWidget->SetIconTexture(Item->GetIconTexture());
			PrintViewport(1.f, FColor::Red, Item->GetNameText());
		}
	}

	else
		m_ItemDescWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryTile::ItemDrag(UObject* Data)
{

}

void UInventoryTile::ShowItem()
{
	m_InventoryTile->ClearListItems();

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	Inventory* Inven = GameInst->GetInventory();

	TArray<FInventoryItem>* InvenArray = Inven->GetInventoryArray();

	for (int i = 0; i < InvenArray->Num(); ++i)
	{
		UInventoryItemDataTile* Data = NewObject<UInventoryItemDataTile>(this,
			UInventoryItemDataTile::StaticClass());

		Data->SetNameText((*InvenArray)[i].Info.Name);
		Data->SetIconTexture((*InvenArray)[i].Info.IconTexture);
		Data->SetIndex(m_InventoryTile->GetNumItems());
		Data->SetKey((*InvenArray)[i].Info.Key);

		if ((*InvenArray)[i].Info.Overlap == EItemOverlap::Overlap)
			Data->SetNumberText((*InvenArray)[i].Number);

		m_InventoryTile->AddItem(Data);
	}
}

void UInventoryTile::SetGold(int32 Gold)
{
	m_GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold : %d"), Gold)));
}

void UInventoryTile::ChangeSlotType()
{
	AUEKR2GameModeBase* GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	UMainHUD* MainHUD = GameMode->GetMainHUD();

	MainHUD->ChangeInventoryType();
}
