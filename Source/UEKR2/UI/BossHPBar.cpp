// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPBar.h"
#include "../UEKR2GameInstance.h"

void UBossHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Bar")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_PercentText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Percent")));

	if (m_NameDelegate.IsBound())
	{
		m_NameDelegate.Execute();
	}
}

void UBossHPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}





