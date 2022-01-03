// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "OtherCharacterUI.generated.h"

DECLARE_DELEGATE(FNameDelegate);

UCLASS()
class UEKR2_API UOtherCharacterUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_CharacterPic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* m_HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProgressBar* m_MPBar;

	FNameDelegate	m_NameDelegate;

	EPlayerJob			m_Job;

public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}

	void SetMPPercent(float Percent)
	{
		m_MPBar->SetPercent(Percent);
	}

	void SetName(const FString& Name)
	{
		m_NameText->SetText(FText::FromString(Name));
	}

	void SetJob(EPlayerJob Job)
	{
		m_Job = Job;
	}

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetUI();

public:
	template <typename T>
	void SetDelegate(T* Obj, void(T::* Func)())
	{
		m_NameDelegate.BindUObject(Obj, Func);
	}
};
