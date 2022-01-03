// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "Engine/GameInstance.h"
#include "../UEKR2GameInstance.h"
#include "PlayerCharacter.h"

Inventory::Inventory() 
	: m_SlotArray{}
{
}

Inventory::~Inventory()
{
}

void Inventory::AddItem(const FUIItemTableInfo* Obj)
{
	switch (Obj->Overlap)
	{
	case EItemOverlap::None:
	{
		return;

		FInventoryItem Item = {};
		Item.Info = *Obj;
		Item.Number = 1;
		m_SlotArray.Add(Item);
	}
	break;

	case EItemOverlap::Overlap:
	{
		// 슬롯찾고 + 더하기
		int32 ItemNumber = FindItem(Obj);

		if (ItemNumber != -1)
			++m_SlotArray[ItemNumber].Number;

		// 없으면 추가
		else
		{
			FInventoryItem Item = {};
			Item.Info = *Obj;
			Item.Number = 1;

			m_SlotArray.Add(Item);
		}
	}
	break;
	}
}

bool Inventory::UseItem(APlayerCharacter* Player, FInventoryItem* Item)
{
	int32 ItemNumber = FindItem(Item);

	if (ItemNumber == -1)
		return false;

	switch (Item->Info.Overlap)
	{
	case EItemOverlap::None:
	{
	}
	break;

	case EItemOverlap::Overlap:
	{
		for (int i = 0; i < m_SlotArray[ItemNumber].Info.OptionArray.Num(); ++i)
		{
			switch (m_SlotArray[ItemNumber].Info.OptionArray[i].OptionType)
			{
			case EItemOption::HPRecovery:
			{
				Player->AddHP(m_SlotArray[ItemNumber].Info.OptionArray[i].Option);
			}
			break;
			case EItemOption::MPRecovery:
			{
				Player->AddMP(m_SlotArray[ItemNumber].Info.OptionArray[i].Option);
			}
			break;
			}
		}


		if (m_SlotArray[ItemNumber].Number > 1)
			--m_SlotArray[ItemNumber].Number;
		else
			m_SlotArray.RemoveAt(ItemNumber);
	}
	break;
	}

	return true;
}

int32 Inventory::FindItem(const FUIItemTableInfo* ObjInfo)
{
	for (int i = 0; i < m_SlotArray.Num(); ++i)
	{
		if (m_SlotArray[i].Info.Key == ObjInfo->Key)
			return i;
	}

	return -1;
}

int32 Inventory::FindItem(FInventoryItem* Item)
{
	for (int i = 0; i < m_SlotArray.Num(); ++i)
	{
		if (m_SlotArray[i].Info.Key == Item->Info.Key)
			return i;
	}

	return -1;
}

int32 Inventory::FindItem(int32 Key)
{
	for (int i = 0; i < m_SlotArray.Num(); ++i)
	{
		if (m_SlotArray[i].Info.Key == Key)
			return i;
	}

	return -1;
}

void Inventory::Reset()
{
	for (int i = 0; i < m_SlotArray.Num(); ++i)
	{
		m_SlotArray.RemoveAt(i);
	}
}