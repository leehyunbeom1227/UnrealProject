// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

class UEKR2_API Inventory
{
public:
	Inventory();
	~Inventory();

private:
	TArray<FInventoryItem> m_SlotArray;

public:
	TArray<FInventoryItem>* GetInventoryArray()
	{
		return &m_SlotArray;
	}

	FInventoryItem* GetInvenItem(int32 SlotNumber)
	{
		return &m_SlotArray[SlotNumber];
	}

	FInventoryItem* GetInvenItemByKey(int32 Key)
	{
		if (FindItem(Key) == -1)
			return nullptr;

		return &m_SlotArray[FindItem(Key)];
	}

public:
	void AddItem(const FUIItemTableInfo* ObjInfo);
	bool UseItem(class APlayerCharacter* Player, FInventoryItem* Item);
	int32 FindItem(const FUIItemTableInfo* ObjInfo);
	int32 FindItem(FInventoryItem* Obj);
	int32 FindItem(int32 Key);

	void Reset();
};
