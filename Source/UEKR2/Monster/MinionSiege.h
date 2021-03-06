// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Minion.h"
#include "MinionSiege.generated.h"

UCLASS()
class UEKR2_API AMinionSiege : public AMinion
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinionSiege();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NormalAttack();

};
