// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_InventoryButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_CharacterStateButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_SkillButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_OptionButton;

	class UInventoryList* m_InventoryList;	
	class UInventoryTile* m_InventoryTile;
	class UCharacterSelectState*	  m_State;

public:
	void SetInventory(class UInventoryList* List)
	{
		m_InventoryList = List;
	}

	void SetState(class UCharacterSelectState* State)
	{
		m_State = State;
	}

public:
	void ChangeSlot();

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void OnInventory();

	UFUNCTION()
		void OnInventoryTile();

	UFUNCTION()
		void OnState();

};
