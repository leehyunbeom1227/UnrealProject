// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UsePotionWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UUsePotionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_Potion1Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_Potion2Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Potion1KeyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Potion2KeyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Potion1NumberText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Potion2NumberText;

	int32 m_Potion1Key;
	int32 m_Potion2Key;

public:
	int32 GetPotionNumber(int32 Key)
	{
		int32 PotionNumber;

		if (Key == 1)
		{
			FText PotionNumberText = m_Potion1NumberText->GetText();
			PotionNumber = FCString::Atof(*PotionNumberText.ToString());
		}
		else if (Key == 2)
		{
			FText PotionNumberText = m_Potion2NumberText->GetText();
			PotionNumber = FCString::Atof(*PotionNumberText.ToString());
		}

		return PotionNumber;
	}

	int32 GetPotionKey(int32 Key)
	{
		if (Key == 1)
			return m_Potion1Key;
		else if (Key == 2)
			return m_Potion2Key;

		return -1;
	}

	void SetKeyText(int32 Key, const FString& NumberText)
	{
		if (Key == m_Potion1Key)
			m_Potion1NumberText->SetText(FText::FromString(NumberText));
		else if (Key == m_Potion2Key)
			m_Potion2NumberText->SetText(FText::FromString(NumberText));
	}

	void SetNumberText(int32 Key, int32 Number)
	{
		if (Key == m_Potion1Key)
			m_Potion1NumberText->SetText(FText::FromString(FString::FromInt(Number)));
		else if (Key == m_Potion2Key)
			m_Potion2NumberText->SetText(FText::FromString(FString::FromInt(Number)));
	}

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void UsePotion1();

	UFUNCTION()
		void UsePotion2();

};
