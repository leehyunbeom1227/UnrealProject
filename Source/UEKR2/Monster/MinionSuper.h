// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Minion.h"
#include "MinionSuper.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AMinionSuper : public AMinion
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinionSuper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NormalAttack();
};
