// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckMonsterHP.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UBTDecorator_CheckMonsterHP : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckMonsterHP();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_CheckHPPercent;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
