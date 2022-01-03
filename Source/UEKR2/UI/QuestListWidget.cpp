// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListWidget.h"


void UQuestListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	m_Selection = Cast<UImage>(GetWidgetFromName(TEXT("Selection")));
	
	m_Select = false;
}

void UQuestListWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestListWidget::SetData(UObject* Data)
{
	UQuestListItemData* Item = Cast<UQuestListItemData>(Data);

	Item->SetOwnerWidget(this);

	m_NameText->SetText(FText::FromString(Item->GetQuestName()));
}

void UQuestListWidget::Select(bool Enable)
{
	m_Select = Enable;

	if (Enable)
	{
		FLinearColor color(0.5f, 0.5f, 0.5f, 0.5f);
		m_Selection->SetBrushTintColor(color);
	}

	else
	{
		FLinearColor color(0.2f, 0.2f, 0.2f, 0.f);
		m_Selection->SetBrushTintColor(color);

	}
}

void UQuestListWidget::MouseOn(bool Enable)
{
	if (!m_Select)
	{
		if (Enable)
		{
			FLinearColor color(0.2f, 0.2f, 0.2f, 0.5f);
			m_Selection->SetBrushTintColor(color);
		}

		else
		{
			FLinearColor color(0.2f, 0.2f, 0.2f, 0.f);
			m_Selection->SetBrushTintColor(color);

		}
	}
}
