// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"
#include "../Player/PlayerCharacter.h"
#include "../UEKR2GameInstance.h"

void UCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_CharacterPic = Cast<UImage>(GetWidgetFromName(TEXT("CharacterPic")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_MPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPBar")));
	
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		SetName(GameInst->GetPlayerName());
		SetHPPercent((float)GameInst->GetPlayerInfo()->HP / (float)GameInst->GetPlayerInfo()->HPMax);
		SetMPPercent((float)GameInst->GetPlayerInfo()->MP / (float)GameInst->GetPlayerInfo()->MPMax);
	}
}

void UCharacterHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
