// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolTimeWidget.h"

void UCoolTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_CoolBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("CoolProgress")));
	m_KeyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Key")));

	m_AccTime = m_MaxTime;

	m_Reverse = false;
}

void UCoolTimeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!m_Reverse)
	{
		if (m_AccTime <= m_MaxTime)
			m_CoolBar->SetPercent(m_AccTime / m_MaxTime);

		else
			m_CoolBar->SetPercent(1.f);
	}

	else
	{
		if (m_AccTime == 0.f)
			m_CoolBar->SetPercent(0.f);

		else if (m_AccTime <= m_MaxTime)
			m_CoolBar->SetPercent(1 - m_AccTime / m_MaxTime);

		else
			m_CoolBar->SetPercent(0.f);
	}
}
