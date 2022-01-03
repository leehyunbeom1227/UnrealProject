// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "InventoryItemList.h"
#include "InventoryItemDataList.h"
#include "Blueprint/UserWidget.h"
#include "InventoryList.generated.h"

//DECLARE_DELEGATE(FInventoryInit)
/**
 * 
 */
UCLASS()
class UEKR2_API UInventoryList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UListView* m_InventoryList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_GoldText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* m_ChangeButton;

	//FInventoryInit m_InvenInitDelegate;
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void ItemDoubleClick(UObject* Data);

	UFUNCTION()
		void ItemDrag(UObject* Data);

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
