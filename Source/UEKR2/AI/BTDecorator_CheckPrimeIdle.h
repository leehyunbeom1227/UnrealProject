// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckPrimeIdle.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API UBTDecorator_CheckPrimeIdle : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckPrimeIdle();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
