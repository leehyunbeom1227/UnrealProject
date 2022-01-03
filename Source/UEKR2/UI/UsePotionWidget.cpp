// Fill out your copyright notice in the Description page of Project Settings.


#include "UsePotionWidget.h"
#include "../UEKR2GameInstance.h"
#include "../Player/Inventory.h"
#include "../Player/PlayerCharacter.h"

void UUsePotionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Potion1Button = Cast<UButton>(GetWidgetFromName(TEXT("Potion1")));
	m_Potion2Button = Cast<UButton>(GetWidgetFromName(TEXT("Potion2")));
	m_Potion1KeyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PotionKey1")));
	m_Potion2KeyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PotionKey2")));
	m_Potion1NumberText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PotionNumber1")));
	m_Potion2NumberText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PotionNumber2")));

	m_Potion1Key = (int32)EItemKey::HPPotion;
	m_Potion2Key = (int32)EItemKey::MPPotion;

	SetKeyText(m_Potion1Key, "F");
	SetKeyText(m_Potion2Key, "G");

	m_Potion1Button->OnClicked.AddDynamic(this, &UUsePotionWidget::UsePotion1);
	m_Potion2Button->OnClicked.AddDynamic(this, &UUsePotionWidget::UsePotion2);

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	Inventory* Inven = GameInst->GetInventory();

	if (Inven->GetInvenItemByKey(m_Potion1Key))
		SetNumberText(m_Potion1Key,Inven->GetInvenItemByKey(m_Potion1Key)->Number);

	else
		SetNumberText(m_Potion1Key, 0);


	if (Inven->GetInvenItemByKey(m_Potion2Key))
		SetNumberText(m_Potion2Key, Inven->GetInvenItemByKey(m_Potion2Key)->Number);

	else
		SetNumberText(m_Potion2Key, 0);
}

void UUsePotionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUsePotionWidget::UsePotion1()
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		Inventory* Inven = GameInst->GetInventory();

		TArray<FInventoryItem>* InvenArray = Inven->GetInventoryArray();

		if (Inven->GetInvenItemByKey(m_Potion1Key))
		{
			if (Inven->UseItem(Player, Inven->GetInvenItemByKey(m_Potion1Key)))
			{
				FText PotionNumberText = m_Potion1NumberText->GetText();
				int32 PotionNumber = FCString::Atof(*PotionNumberText.ToString());

				if (PotionNumber >= 1)
					PotionNumber -= 1;

				SetNumberText(m_Potion1Key, PotionNumber);
			}
		}
	}
}

void UUsePotionWidget::UsePotion2()
{
	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		Inventory* Inven = GameInst->GetInventory();

		if (Inven->UseItem(Player, Inven->GetInvenItemByKey(m_Potion2Key)))
		{
			FText PotionNumberText = m_Potion2NumberText->GetText();
			int32 PotionNumber = FCString::Atof(*PotionNumberText.ToString());

			if (PotionNumber >= 1)
				PotionNumber -= 1;

			SetNumberText(m_Potion2Key, PotionNumber);
		}
	}
}
