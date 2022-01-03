// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroWidget.h"


void UIntroWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Scene = Cast<UImage>(GetWidgetFromName(TEXT("Screen")));
	m_SkipButton = Cast<UButton>(GetWidgetFromName(TEXT("SkipButton")));

	m_SkipButton->OnClicked.AddDynamic(this, &UIntroWidget::SkipButton);
}

void UIntroWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UIntroWidget::SkipButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}



