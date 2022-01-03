// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "InventoryList.h"
#include "InventoryTile.h"
#include "CharacterHUD.h"
#include "MainMenuWidget.h"
#include "QuestWidget.h"
#include "MinimapWidget.h"
#include "BossHPBar.h"
#include "CoolTimeWidget.h"
#include "UseItemWidget.h"
#include "UsePotionWidget.h"
#include "OtherCharacterUI.h"
#include "CharacterSelectState.h"
#include "Components/Slider.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCharacterHUD* m_CharacterHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMainMenuWidget* m_MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UInventoryList* m_InventoryList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UInventoryTile* m_InventoryTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UQuestWidget* m_QuestWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMinimapWidget* m_MinimapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBossHPBar*		m_BossHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUseItemWidget* m_UseItemWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCoolTimeWidget* m_Skill1CoolWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCoolTimeWidget* m_Skill2CoolWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCoolTimeWidget* m_DashCoolWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCoolTimeWidget* m_BufCoolWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCoolTimeWidget* m_ChangeCoolWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UUsePotionWidget* m_UsePotionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UOtherCharacterUI* m_OtherCharacterUI1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UOtherCharacterUI* m_OtherCharacterUI2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UOtherCharacterUI* m_OtherCharacterUI3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCharacterSelectState* m_SelectState;

	class UMaterialParameterCollection* m_LandScapeCollection;
	class UMaterialParameterCollectionInstance* m_LandScapeCollectionInst;

public:
	UCharacterSelectState* GetSelectState()
	{
		return m_SelectState;
	}

	UUsePotionWidget* GetUsePotionWidget()
	{
		return m_UsePotionWidget;
	}

	UCharacterHUD* GetCharacterHUD()
	{
		return m_CharacterHUD;
	}

	UMainMenuWidget* GetMainMenuHUD()
	{
		return m_MainMenu;
	}

	UInventoryList* GetInventoryList()
	{
		return m_InventoryList;
	}

	UInventoryTile* GetInventoryTile()
	{
		return m_InventoryTile;
	}

	UQuestWidget* GetQuest()
	{
		return m_QuestWidget;
	}

	UMinimapWidget* GetMinimap()
	{
		return m_MinimapWidget;
	}

	UBossHPBar* GetBossHPBar()
	{
		return m_BossHPBar;
	}

	UCoolTimeWidget* GetSkill1CoolWidget()
	{
		return m_Skill1CoolWidget;
	}

	UCoolTimeWidget* GetSkill2CoolWidget()
	{
		return m_Skill2CoolWidget;
	}

	UCoolTimeWidget* GetDashCoolWidget()
	{
		return m_DashCoolWidget;
	}

	UCoolTimeWidget* GetBufCoolWidget()
	{
		return m_BufCoolWidget;
	}

	UCoolTimeWidget* GetChangeCoolWidget()
	{
		return m_ChangeCoolWidget;
	}

	UUseItemWidget* GetUseItemWidget()
	{
		return m_UseItemWidget;
	}

	UOtherCharacterUI* GetOtherCharacterUI(int32 Number)
	{
		switch (Number)
		{
		case 1:
			return m_OtherCharacterUI1;
			break;
		case 2:
			return m_OtherCharacterUI2;
			break;
		case 3:
			return m_OtherCharacterUI3;
			break;
		}

		return nullptr;
	}
	

public:
	void ChangeInventoryType();
	void ChangeBossHPBarType();

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

};
