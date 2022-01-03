// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryList.h"
#include "InventoryItemDataList.h"
#include "../UEKR2GameInstance.h"
#include "../UEKR2GameModeBase.h"
#include "MainHUD.h"
#include "UsePotionWidget.h"
#include "../Player/Inventory.h"
#include "../Player/PlayerCharacter.h"


void UInventoryList::NativeConstruct()
{
	Super::NativeConstruct();

	m_InventoryList = Cast<UListView>(GetWidgetFromName(TEXT("InventoryList")));
	m_GoldText		= Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldText")));
	m_ChangeButton  = Cast<UButton>(GetWidgetFromName(TEXT("ChangeButton")));
	
	//if (m_InvenInitDelegate.IsBound())
	//	m_InvenInitDelegate.Execute();

	/*
	FString ItemNameArray[3] =
	{
		TEXT("BF대검"),
		TEXT("칠흑의양날도끼"),
		TEXT("HP포션")
	};

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	for (int32 i = 0; i < 100; ++i)
	{
		int32 Index = FMath::RandRange(0, 2);

		const FUIItemTableInfo* Info = GameInst->FindUIItemInfo(ItemNameArray[Index]);

		UInventoryItemDataList* Data = NewObject<UInventoryItemDataList>(this,
			UInventoryItemDataList::StaticClass());

		Data->SetNameText(Info->Name);
		Data->SetIcon(Info->IconTexture);
		Data->SetIndex(i);

		m_InventoryList->AddItem(Data);
	}*/
	m_ChangeButton->OnClicked.AddDynamic(this, &UInventoryList::ChangeSlotType);
	m_InventoryList->OnItemDoubleClicked().AddUObject(this, &UInventoryList::ItemDoubleClick);
	/*
	* m_InventoryList->OnItemSelectionChanged()
	* m_InventoryList->OnItemDoubleClicked()
	* m_InventoryList->OnItemIsHoveredChanged()
	*/
}

void UInventoryList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryList::ItemDoubleClick(UObject* Data)
{
	UInventoryItemDataList* Item = Cast<UInventoryItemDataList>(Data);

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

void UInventoryList::ShowItem()
{
	m_InventoryList->ClearListItems();

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());
	
	if (GameInst)
	{
		Inventory* Inven = GameInst->GetInventory();

		TArray<FInventoryItem>* InvenArray = Inven->GetInventoryArray();

		for (int i = 0; i < InvenArray->Num(); ++i)
		{
			UInventoryItemDataList* Data = NewObject<UInventoryItemDataList>(this,
				UInventoryItemDataList::StaticClass());

			Data->SetNameText((*InvenArray)[i].Info.Name);
			Data->SetIcon((*InvenArray)[i].Info.IconTexture);
			Data->SetIndex(m_InventoryList->GetNumItems());
			Data->SetKey((*InvenArray)[i].Info.Key);

			if ((*InvenArray)[i].Info.Overlap == EItemOverlap::Overlap)
				Data->SetNumberText((*InvenArray)[i].Number);


			m_InventoryList->AddItem(Data);
		}
	}
}

void UInventoryList::ItemDrag(UObject* Data)
{

}

void UInventoryList::SetGold(int32 Gold)
{
	m_GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold : %d"), Gold)));
}

void UInventoryList::ChangeSlotType()
{
	// 슬롯 타입 바꾸기
	AUEKR2GameModeBase * GameMode = Cast<AUEKR2GameModeBase>(GetWorld()->GetAuthGameMode());

	UMainHUD* MainHUD = GameMode->GetMainHUD();

	MainHUD->ChangeInventoryType();
}
