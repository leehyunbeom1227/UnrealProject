// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Effect/ParticleProjectile.h"
#include "PhaseSkill2.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API APhaseSkill2 : public AParticleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APhaseSkill2();

private:
	int32		  m_Attack;
	class APhase* m_Owner;
	FVector		  m_DirVector;
	int32		  m_Number;

public:
	void SetDir(FVector Dir)
	{
		m_DirVector = Dir;
	}

	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class APhase* Phase)
	{
		m_Owner = Phase;
	}

	void SetNumber(int32 Number)
	{
		m_Number = Number;
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
