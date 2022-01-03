// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "InventoryList.h"
#include "InventoryTile.h"
#include "QuestWidget.h"
#include "CharacterSelectState.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_InventoryButton = Cast<UButton>(GetWidgetFromName(TEXT("InventoryButton")));
	m_CharacterStateButton = Cast<UButton>(GetWidgetFromName(TEXT("CharacterStatusButton")));

	m_InventoryButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInventory);
	m_CharacterStateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnState);
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainMenuWidget::OnInventory()
{
	if (m_InventoryList->GetVisibility() == ESlateVisibility::Collapsed)
	{
		m_InventoryList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_InventoryList->ShowItem();
	}
	else
		m_InventoryList->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OnInventoryTile()
{
	if (m_InventoryTile->GetVisibility() == ESlateVisibility::Collapsed)
	{
		m_InventoryTile->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_InventoryTile->ShowItem();
	}
	else
		m_InventoryTile->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OnState()
{
	if (m_State->GetVisibility() == ESlateVisibility::Collapsed)
	{
		m_State->SetVisibility(ESlateVisibility::Visible);
		m_State->SetUI();
	}
	else
		m_State->SetVisibility(ESlateVisibility::Collapsed);
}
