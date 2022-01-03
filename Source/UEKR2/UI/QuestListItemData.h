// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "QuestListItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UEKR2_API UQuestListItemData : public UObject
{
	GENERATED_BODY()

public:
	UQuestListItemData();

private:
	FString m_Name;
	int32	m_Index;
	class UQuestListWidget* m_OwnerWidget;

public:
	class UQuestListWidget* GetOwnerWidget()
	{
		return m_OwnerWidget;
	}

	void SetOwnerWidget(class UQuestListWidget* Widget)
	{
		m_OwnerWidget = Widget;
	}

	void SetIndex(int32 Index)
	{
		m_Index = Index;
	}

	void SetQuestName(const FString& Name)
	{
		m_Name = Name;
	}

	int32 GetIndex()
	{
		return m_Index;
	}

	const FString& GetQuestName() const
	{
		return m_Name;
	}


};
