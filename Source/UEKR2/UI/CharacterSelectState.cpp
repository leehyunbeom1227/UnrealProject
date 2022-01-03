// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectState.h"
#include "../UEKR2GameInstance.h"
#include "../Player/PlayerCharacter.h"

void UCharacterSelectState::NativeConstruct()
{
	Super::NativeConstruct();

	m_CharacterImage = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImage")));
	m_NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_JobText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Job")));
	m_AttackText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Attack")));
	m_ArmorText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Armor")));
	m_HPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HP")));
	m_MPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MP")));
	m_Skill1Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Skill1")));
	m_Skill2Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Skill2")));

	UMaterialInterface* ImageMaterial = LoadObject<UMaterialInterface>(nullptr,
		TEXT("MaterialInstanceConstant'/Game/Player/MTUIRenderTargetDepthClear_PlayerBody.MTUIRenderTargetDepthClear_PlayerBody'"));

	if (IsValid(ImageMaterial))
		m_ImageInterface = ImageMaterial;
}

void UCharacterSelectState::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCharacterSelectState::SetImageMaterial(UMaterialInterface* Material)
{
	m_CharacterImage->SetBrushFromMaterial(Material);
}

void UCharacterSelectState::SetSkiil1Text()
{

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		int32 PlayerSkill1Attack = Player->GetPlayerInfo().Attack * Player->GetSkill1Attack();

		m_Skill1Text->SetText(FText::FromString(FString::Printf(TEXT("Skill1 : %d"), PlayerSkill1Attack)));
	}

}

void UCharacterSelectState::SetSkiil2Text()
{
		UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

		if (GameInst)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

			switch (GameInst->GetSelectJob())
			{
			case EPlayerJob::Knight:
			{
				int32 PlayerSkill = Player->GetSkill1Attack();

				m_Skill2Text->SetText(FText::FromString(FString::Printf(TEXT("Buff : Attack Multi %d Buf"), PlayerSkill)));
			}
				break;

			case EPlayerJob::Archer:
			{
				int32 PlayerSkill= Player->GetPlayerInfo().Attack * Player->GetSkill1Attack();

				m_Skill2Text->SetText(FText::FromString(FString::Printf(TEXT("Drone : Attack %d"), PlayerSkill)));
			}
				break;

			case EPlayerJob::Magicion:		
			{
				int32 PlayerSkill = Player->GetPlayerInfo().Attack * Player->GetSkill2Attack();

				m_Skill2Text->SetText(FText::FromString(FString::Printf(TEXT("Skill2 : %d"), PlayerSkill)));
			}
				break;

			case EPlayerJob::Assassin:	
			{
				int32 PlayerSkill = Player->GetPlayerInfo().Attack * Player->GetSkill2Attack();

				m_Skill2Text->SetText(FText::FromString(FString::Printf(TEXT("Skill2 : %d"), PlayerSkill)));
			}
				break;
			}
		}
}

void UCharacterSelectState::SetUI()
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		FPlayerInfo* Info = GameInst->GetPlayerInfo();

		SetImageMaterial(m_ImageInterface);
		SetNameText(Info->Name);
		SetJobText(Info->Job);
		SetAttackText(Info->Attack);
		SetArmorText(Info->Armor);
		SetHPText(Info->HP, Info->HPMax);
		SetMPText(Info->MP, Info->MPMax);
		SetSkiil1Text();
		SetSkiil2Text();
	}
}
