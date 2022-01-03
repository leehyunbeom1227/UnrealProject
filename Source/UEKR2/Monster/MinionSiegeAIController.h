// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "MinionAIController.h"
#include "MinionSiegeAIController.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AMinionSiegeAIController : public AMinionAIController
{
	GENERATED_BODY()

public:
	AMinionSiegeAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
