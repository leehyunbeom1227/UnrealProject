// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemDataTile.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UInventoryItemDataTile : public UObject
{
	GENERATED_BODY()

public:
	UInventoryItemDataTile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTexture2D* m_IconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_NumberText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 m_Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 m_Key;

public:
	FString GetNumberText()
	{
		return m_NumberText;
	}

	UTexture2D* GetIconTexture()
	{
		return m_IconTexture;
	}

	FString GetNameText()
	{
		return m_NameText;
	}

	int32 GetIndex()
	{
		return m_Index;
	}

	int32 GetKey()
	{
		return m_Key;
	}

	void SetNumberText(int32 Number)
	{
		m_NumberText = FString::FromInt(Number);
	}

	void SetIconTexture(UTexture2D* IconTex)
	{
		m_IconTexture = IconTex;
	}

	void SetNameText(const FString& NameText)
	{
		m_NameText = NameText;
	}

	void SetIndex(int32 Index)
	{
		m_Index = Index;
	}

	void SetKey(int32 Key)
	{
		m_Key = Key;
	}
};
