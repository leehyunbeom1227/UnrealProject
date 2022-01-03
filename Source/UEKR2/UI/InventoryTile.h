// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TileView.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "InventoryItemTile.h"
#include "InventoryItemDataTile.h"
#include "ItemDescWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTile.generated.h"

//DECLARE_DELEGATE(FInventoryInit)
/**
 * 
 */
UCLASS()
class UEKR2_API UInventoryTile : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* m_InventoryTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* m_InventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UItemDescWidget* m_ItemDescWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_GoldText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_ChangeButton;


	//FInventoryInit m_InvenInitDelegate;

	int32 m_ItemCount;
	bool  m_MouseHovered;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);


public:
	UFUNCTION()
		void ItemDoubleClick(UObject* Data);

	UFUNCTION()
		void ItemDrag(UObject* Data);

	UFUNCTION()
		void ItemHOvered(UObject* Data, bool Hovered);

	UFUNCTION()
		void ChangeSlotType();

public:
	void ShowItem();
	void SetGold(int32 Gold);
	//
	//public:
	//	template <typename T>
	//	void SetInventoryInitDelegate(T* Obj, void(T::* Func)())
	//	{
	//		m_InvenInitDelegate.BindUObject(Obj, Func);
	//	}

};
