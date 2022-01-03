// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "../UEKR2GameInstance.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_CharacterHUD = Cast<UCharacterHUD>(GetWidgetFromName(TEXT("UI_CharacterHUD")));
	m_MainMenu = Cast<UMainMenuWidget>(GetWidgetFromName(TEXT("UI_MainMenu")));
	m_InventoryList = Cast<UInventoryList>(GetWidgetFromName(TEXT("UI_InventoryList")));
	m_InventoryTile = Cast<UInventoryTile>(GetWidgetFromName(TEXT("UI_InventoryTile")));
	m_QuestWidget = Cast<UQuestWidget>(GetWidgetFromName(TEXT("UI_Quest")));
	m_MinimapWidget = Cast<UMinimapWidget>(GetWidgetFromName(TEXT("UI_Minimap")));
	m_BossHPBar = Cast<UBossHPBar>(GetWidgetFromName(TEXT("UI_BossHPBar")));
	m_Skill1CoolWidget = Cast<UCoolTimeWidget>(GetWidgetFromName(TEXT("UI_Skill1Cool")));
	m_Skill2CoolWidget = Cast<UCoolTimeWidget>(GetWidgetFromName(TEXT("UI_Skill2Cool")));
	m_DashCoolWidget = Cast<UCoolTimeWidget>(GetWidgetFromName(TEXT("UI_DashCool")));
	m_BufCoolWidget = Cast<UCoolTimeWidget>(GetWidgetFromName(TEXT("UI_BufCool")));
	m_ChangeCoolWidget = Cast<UCoolTimeWidget>(GetWidgetFromName(TEXT("UI_ChangeCool")));
	m_UsePotionWidget = Cast<UUsePotionWidget>(GetWidgetFromName(TEXT("UI_UsePotion")));
	m_OtherCharacterUI1 = Cast<UOtherCharacterUI>(GetWidgetFromName(TEXT("UI_OtherCharacterUI1")));
	m_OtherCharacterUI2 = Cast<UOtherCharacterUI>(GetWidgetFromName(TEXT("UI_OtherCharacterUI2")));
	m_OtherCharacterUI3 = Cast<UOtherCharacterUI>(GetWidgetFromName(TEXT("UI_OtherCharacterUI3")));
	m_SelectState = Cast<UCharacterSelectState>(GetWidgetFromName(TEXT("UI_SelectState")));

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	m_CharacterHUD->SetName(GameInst->GetPlayerInfo()->Name);

	EPlayerJob SelectJob = GameInst->GetSelectJob();
	SelectJob = ++SelectJob;
	m_OtherCharacterUI1->SetJob(SelectJob);
	SelectJob = ++SelectJob;
	m_OtherCharacterUI2->SetJob(SelectJob);
	SelectJob = ++SelectJob;
	m_OtherCharacterUI3->SetJob(SelectJob);

	m_OtherCharacterUI1->SetUI();
	m_OtherCharacterUI2->SetUI();
	m_OtherCharacterUI3->SetUI();

	m_BufCoolWidget->SetReverse();
	m_Skill1CoolWidget->SetMaxTime(5.f);
	m_Skill2CoolWidget->SetMaxTime(7.f);
	m_DashCoolWidget->SetMaxTime(5.f);
	m_BufCoolWidget->SetMaxTime(60.f);
	m_ChangeCoolWidget->SetMaxTime(10.f);

	m_MainMenu->SetInventory(m_InventoryList);
	m_MainMenu->SetState(m_SelectState);
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UMainHUD::ChangeInventoryType()
{
	if (m_InventoryTile->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_InventoryTile->SetVisibility(ESlateVisibility::Collapsed);
		m_InventoryList->RenderTransform.Translation = m_InventoryTile->RenderTransform.Translation;
		m_InventoryList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_InventoryList->ShowItem();
	}

	else if (m_InventoryList->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		m_InventoryList->SetVisibility(ESlateVisibility::Collapsed);		
		m_InventoryTile->RenderTransform.Translation = m_InventoryList->RenderTransform.Translation;
		m_InventoryTile->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_InventoryTile->ShowItem();
	}
}

void UMainHUD::ChangeBossHPBarType()
{
	if (m_BossHPBar->GetVisibility() == ESlateVisibility::Visible)
		m_BossHPBar->SetVisibility(ESlateVisibility::Collapsed);

	else
		m_BossHPBar->SetVisibility(ESlateVisibility::Visible);
}
