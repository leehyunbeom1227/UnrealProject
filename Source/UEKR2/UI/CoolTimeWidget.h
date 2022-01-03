// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CoolTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UCoolTimeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* m_CoolBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_MaxTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_AccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_KeyText;

	bool m_Reverse;

public:
	void SetKey(FString& Key)
	{
		m_KeyText->SetText(FText::FromString(Key));
	}

	void SetTime(float Time)
	{
		m_AccTime = Time;
	}

	void SetMaxTime(float MaxTime)
	{
		m_MaxTime = MaxTime;
	}
	
	bool GetFinish()
	{
		if (m_AccTime / m_MaxTime >= 1) 
			return true;
		else 
			return false;
	}

	void SetReverse()
	{
		m_Reverse = true;
	}

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

};
