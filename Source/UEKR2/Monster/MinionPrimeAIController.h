// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "MinionAIController.h"
#include "MinionPrimeAIController.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AMinionPrimeAIController : public AMinionAIController
{
	GENERATED_BODY()

public:
	AMinionPrimeAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
