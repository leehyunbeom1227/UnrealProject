// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "MonsterAIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API ABossAIController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	ABossAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
