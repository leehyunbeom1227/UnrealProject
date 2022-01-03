// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ParticleProjectile.h"
#include "BossTeleport.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API ABossTeleport : public AParticleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossTeleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
