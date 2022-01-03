// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ParticleProjectile.h"
#include "ShinbiSkill2Boom.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AShinbiSkill2Boom : public AParticleProjectile
{
	GENERATED_BODY()

public:
	AShinbiSkill2Boom();

private:
	int32	m_Attack;
	class AShinbi* m_Owner;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AShinbi* Shinbi)
	{
		m_Owner = Shinbi;
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
