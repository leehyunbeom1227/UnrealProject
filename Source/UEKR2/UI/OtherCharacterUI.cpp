// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherCharacterUI.h"
#include "../Player/PlayerCharacter.h"
#include "../UEKR2GameInstance.h"

void UOtherCharacterUI::NativeConstruct()
{
	Super::NativeConstruct();

	m_CharacterPic = Cast<UImage>(GetWidgetFromName(TEXT("CharacterPic")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_MPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPBar")));

}

void UOtherCharacterUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UOtherCharacterUI::SetUI()
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		m_CharacterPic->SetBrushFromTexture(GameInst->GetCapture(m_Job));
		SetName(GameInst->GetPlayerInfo(m_Job)->Name);
		
		if (GameInst->GetPlayerInfo(m_Job)->HP > 0)
		{
			SetHPPercent((float)GameInst->GetPlayerInfo(m_Job)->HP / (float)GameInst->GetPlayerInfo(m_Job)->HPMax);
			SetMPPercent((float)GameInst->GetPlayerInfo(m_Job)->MP / (float)GameInst->GetPlayerInfo(m_Job)->MPMax);
			SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}

		else
		{
			SetHPPercent(0);
			SetMPPercent(0);
			SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));
		}
	}
}

