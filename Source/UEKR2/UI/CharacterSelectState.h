﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectState.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UCharacterSelectState : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* m_CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_JobText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_AttackText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_ArmorText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_HPText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_MPText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Skill1Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Skill2Text;

	UMaterialInterface* m_ImageInterface;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetImageMaterial(UMaterialInterface* Material);
	void SetNameText(const FString& Name)
	{
		m_NameText->SetText(FText::FromString(Name));
	}

	void SetJobText(EPlayerJob Job)
	{
		FString	JobName;
		switch (Job)
		{
		case EPlayerJob::Knight:
			JobName = TEXT("Job : Knight");
			break;
		case EPlayerJob::Archer:
			JobName = TEXT("Job : Archer");
			break;
		case EPlayerJob::Magicion:
			JobName = TEXT("Job : Magicion");
			break;
		}

		m_JobText->SetText(FText::FromString(JobName));
	}

	void SetAttackText(int32 Attack)
	{
		m_AttackText->SetText(FText::FromString(FString::Printf(TEXT("Attack : %d"), Attack)));
	}

	void SetArmorText(int32 Armor)
	{
		m_ArmorText->SetText(FText::FromString(FString::Printf(TEXT("Armor : %d"), Armor)));
	}

	void SetHPText(int32 HP, int32 HPMax)
	{
		m_HPText->SetText(FText::FromString(FString::Printf(TEXT("HP : %d / %d"), HP, HPMax)));
	}

	void SetMPText(int32 MP, int32 MPMax)
	{
		m_MPText->SetText(FText::FromString(FString::Printf(TEXT("MP : %d / %d"), MP, MPMax)));
	}

	void SetSkiil1Text();

	void SetSkiil2Text();

	void SetUI();
};
