// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckBossEvasion.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UBTDecorator_CheckBossEvasion : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckBossEvasion();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
