// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Effect/ParticleProjectile.h"
#include "PhaseSkill1.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API APhaseSkill1 : public AParticleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APhaseSkill1();

private:
	int32	m_Attack;
	class APhase* m_Owner;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class APhase* Phase)
	{
		m_Owner = Phase;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void StopEvent(const FHitResult& result);
};