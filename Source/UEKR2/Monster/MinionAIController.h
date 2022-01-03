// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAIController.h"
#include "MinionAIController.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AMinionAIController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	AMinionAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
