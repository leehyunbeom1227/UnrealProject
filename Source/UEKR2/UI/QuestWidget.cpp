// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"
#include "../UEKR2GameInstance.h"
#include "QuestListItemData.h"
#include "QuestListWidget.h"
#include "../Player/PlayerCharacter.h"
#include "InventoryList.h"
#include "InventoryTile.h"
#include "MainHUD.h"

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_QuestList = Cast<UListView>(GetWidgetFromName(TEXT("QuestList")));
	m_QuestDesc = Cast<UQuestDescWidget>(GetWidgetFromName(TEXT("UI_QuestDesc")));
	
	m_QuestList->OnItemClicked().AddUObject(this, &UQuestWidget::QuestClick);
	m_QuestList->OnItemSelectionChanged().AddUObject(this, &UQuestWidget::QuestSelect);
	m_QuestList->OnItemIsHoveredChanged().AddUObject(this, &UQuestWidget::QuestMouseOn);

	m_CurrentRenderQuest = nullptr;

	UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());
	
	if (GameInst)
	{
		const TMap<int32, FQuestData>& QuestData = GameInst->GetQuestData();

		int32 Index = 0;

		for (auto& Data : QuestData)
		{
			const FQuestTableInfo* Info = GameInst->FindQuestInfo(Data.Key);

			if (Info)
			{
				m_QuestArray.Add(Data.Value);

				int32 QuestIndex = m_QuestArray.Num() - 1;

				for (auto& QuestInfo : Info->InfoArray)
				{
					m_QuestListArray[(uint8)QuestInfo.Type].Add(&m_QuestArray[QuestIndex]);
				}

				UQuestListItemData* QusetListItem = NewObject<UQuestListItemData>(this,
					UQuestListItemData::StaticClass());

				QusetListItem->SetIndex(Index);
				QusetListItem->SetQuestName(Info->Name);

				m_QuestList->AddItem(QusetListItem);

				++Index;
			}
		}
	}

	m_PrevSelect = nullptr;
}

void UQuestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestWidget::QuestClick(UObject* Data)
{
	UQuestListItemData* Info = Cast<UQuestListItemData>(Data);

	if (Info)
	{
		if (m_CurrentRenderQuest == &m_QuestArray[Info->GetIndex()])
		{
			m_QuestDesc->SetQuestName(TEXT(""));
			m_QuestDesc->SetQuestDesc(TEXT(""));
			m_QuestDesc->ClearList();
			m_CurrentRenderQuest = nullptr;
		
			return;
		}

		m_CurrentRenderQuest = &m_QuestArray[Info->GetIndex()];

		FString QuestName = m_QuestArray[Info->GetIndex()].Name;
		
		if (m_CurrentRenderQuest->Complete)
			QuestName = m_QuestArray[Info->GetIndex()].Name + TEXT("(완료)");

		m_QuestDesc->SetQuestName(QuestName);
		m_QuestDesc->SetQuestDesc(m_QuestArray[Info->GetIndex()].QuestDesc);

		m_QuestDesc->ClearList();

		int32 Index = 0;

		for (auto& DataInfo : m_QuestArray[Info->GetIndex()].CompleteArray)
		{
			m_QuestDesc->AddList(DataInfo, Index);
			++Index;
		}
	}

}

void UQuestWidget::QuestSelect(UObject* Data)
{
	// Selection을 클릭시 Item이 nullptr
	UQuestListItemData* Item = Cast<UQuestListItemData>(Data);

	if (m_PrevSelect == Item)
	{
		m_PrevSelect->GetOwnerWidget()->Select(false);
		m_PrevSelect = nullptr;
	}

	if (m_PrevSelect)
		m_PrevSelect->GetOwnerWidget()->Select(false);

	if (Item)
	{
		Item->GetOwnerWidget()->Select(true);

		m_PrevSelect = Item;
	}

	else
		m_PrevSelect = nullptr;
}

void UQuestWidget::QuestMouseOn(UObject* Data, bool IsHovered)
{
	UQuestListItemData* Item = Cast<UQuestListItemData>(Data);
	
	Item->GetOwnerWidget()->MouseOn(IsHovered);
}

void UQuestWidget::QuestCheck(EQuestType Type, const FString& Name)
{
	for (auto& Info : m_QuestListArray[(uint8)Type])
	{
		if (Info->Complete)
			continue;

		int32 CompleteCount = 0;

		for (auto& QuestData : Info->CompleteArray)
		{
			if (QuestData.Complete)
			{
				++CompleteCount;
				continue;
			}

			if (QuestData.Type == Type && QuestData.DestName == Name)
			{
				++QuestData.Count;

				if (QuestData.Count == QuestData.MaxCount)
				{
					++CompleteCount;
					QuestData.Complete = true;
				}

				// 현재 퀘스트창에 보여지고 있는 퀘스트가 변경이 일어났을 경우 갱신
				if (m_CurrentRenderQuest == Info)
				{
					m_QuestDesc->SetQuestName(m_CurrentRenderQuest->Name);
					m_QuestDesc->SetQuestDesc(m_CurrentRenderQuest->QuestDesc);

					m_QuestDesc->ClearList();

					int32 CompleteIndex = 0;

					for (auto& DataInfo : m_CurrentRenderQuest->CompleteArray)
					{
						m_QuestDesc->AddList(DataInfo, CompleteIndex);
						++CompleteIndex;
					}
				}
			}
		}

		if (CompleteCount == Info->CompleteArray.Num())
		{
			Info->Complete = true;

			for (auto& CompensationData : Info->CompensationArray)
			{
				APlayerCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
				
				if (Player)
				{
					switch (CompensationData.Type)
					{
					case EQuestCompensationType::Gold:
					{
						int32 Gold = FCString::Atoi(*CompensationData.Compensation);
						Player->AddGold(Gold);
					}
					break;
					case EQuestCompensationType::Item:
					{
						UUEKR2GameInstance* GameInst = Cast<UUEKR2GameInstance>(GetWorld()->GetGameInstance());

						if (GameInst)
						{
							const FUIItemTableInfo* ItemInfo = GameInst->FindUIItemInfo(CompensationData.Key);

							if (ItemInfo)
							{
								Inventory* Inven = GameInst->GetInventory();
								Inven->AddItem(ItemInfo);
							}
						}
					}
					break;
					case EQuestCompensationType::Exp:
					{
						int32 Exp = FCString::Atoi(*CompensationData.Compensation);
						Player->AddExp(Exp);
					}
					break;
					}
				}
			}
		}
	}
}
