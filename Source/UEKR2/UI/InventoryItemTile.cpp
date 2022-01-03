// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemTile.h"
#include "InventoryItemDataTile.h"

void UInventoryItemTile::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	m_ItemNumberText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber")));
}

void UInventoryItemTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryItemTile::SetData(UObject* Data)
{
	UInventoryItemDataTile* InvenData = Cast<UInventoryItemDataTile>(Data);

	UTexture2D* IconTexture = InvenData->GetIconTexture();

	m_Index = InvenData->GetIndex();

	m_Key = InvenData->GetKey();

	if (IconTexture)
		m_IconImage->SetBrushFromTexture(IconTexture);

	if (InvenData->GetNumberText() != "")
		m_ItemNumberText->SetText(FText::FromString(InvenData->GetNumberText()));
}
