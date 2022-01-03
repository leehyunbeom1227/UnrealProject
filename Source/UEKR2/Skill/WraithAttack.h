// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Gameinfo.h"
#include "../Effect/ParticleProjectile.h"
#include "WraithAttack.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AWraithAttack : public AParticleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWraithAttack();

private:
	int32			m_Attack;
	class AWraith* m_Owner;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AWraith* Wraith)
	{
		m_Owner = Wraith;
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
