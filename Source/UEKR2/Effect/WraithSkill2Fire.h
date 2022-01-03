// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ParticleProjectile.h"
#include "WraithSkill2Fire.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AWraithSkill2Fire : public AParticleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWraithSkill2Fire();

private:
	int32			m_Attack;
	class AWraith* m_Owner;
	FVector			m_Dir;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AWraith* Wraith)
	{
		m_Owner = Wraith;
	}

	void SetDir(FVector Dir)
	{
		m_Dir = Dir;
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
